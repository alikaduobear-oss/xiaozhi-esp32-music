#include "application.h"
#include "board.h"
#include "display.h"
#include "system_info.h"
#include "audio_codec.h"
#include "mqtt_protocol.h"
#include "websocket_protocol.h"
#include "font_awesome_symbols.h"
#include "assets/lang_config.h"
#include "mcp_server.h"

#include <cstring>
#include <esp_log.h>
#include <cJSON.h>
#include <driver/gpio.h>
#include <arpa/inet.h>

#define TAG "Application"

static const char *const STATE_STRINGS[] = {
    "unknown",
    "starting",
    "configuring",
    "idle",
    "connecting",
    "listening",
    "speaking",
    "upgrading",
    "activating",
    "audio_testing",
    "fatal_error",
    "invalid_state"};

Application::Application()
{
    event_group_ = xEventGroupCreate();

#if CONFIG_USE_DEVICE_AEC && CONFIG_USE_SERVER_AEC
#error "CONFIG_USE_DEVICE_AEC and CONFIG_USE_SERVER_AEC cannot be enabled at the same time"
#elif CONFIG_USE_DEVICE_AEC
    aec_mode_ = kAecOnDeviceSide;
#elif CONFIG_USE_SERVER_AEC
    aec_mode_ = kAecOnServerSide;
#else
    aec_mode_ = kAecOff;
#endif

    esp_timer_create_args_t clock_timer_args = {
        .callback = [](void *arg)
        {
            Application *app = (Application *)arg;
            app->OnClockTimer();
        },
        .arg = this,
        .dispatch_method = ESP_TIMER_TASK,
        .name = "clock_timer",
        .skip_unhandled_events = true};
    esp_timer_create(&clock_timer_args, &clock_timer_handle_);
}

Application::~Application()
{
    if (clock_timer_handle_ != nullptr)
    {
        esp_timer_stop(clock_timer_handle_);
        esp_timer_delete(clock_timer_handle_);
    }
    vEventGroupDelete(event_group_);
}

void Application::CheckNewVersion(Ota &ota)
{
    const int MAX_RETRY = 10;
    int retry_count = 0;
    int retry_delay = 10;

    auto &board = Board::GetInstance();
    while (true)
    {
        SetDeviceState(kDeviceStateActivating);
        auto display = board.GetDisplay();
        display->SetStatus(Lang::Strings::CHECKING_NEW_VERSION);

        if (!ota.CheckVersion())
        {
            retry_count++;
            if (retry_count >= MAX_RETRY)
            {
                ESP_LOGE(TAG, "Too many retries, exit version check");
                return;
            }

            char buffer[128];
            snprintf(buffer, sizeof(buffer), Lang::Strings::CHECK_NEW_VERSION_FAILED, retry_delay, ota.GetCheckVersionUrl().c_str());
            // 【注释】暂时没有 exclamation.p3
            Alert(Lang::Strings::ERROR, buffer, "sad", Lang::Sounds::P3_WELCOME); // 临时用 welcome 替代

            ESP_LOGW(TAG, "Check new version failed, retry in %d seconds (%d/%d)", retry_delay, retry_count, MAX_RETRY);
            for (int i = 0; i < retry_delay; i++)
            {
                vTaskDelay(pdMS_TO_TICKS(1000));
                if (device_state_ == kDeviceStateIdle)
                {
                    break;
                }
            }
            retry_delay *= 2;
            continue;
        }
        retry_count = 0;
        retry_delay = 10;

        if (ota.HasNewVersion())
        {
            // 【注释】暂时没有 upgrade.p3
            Alert(Lang::Strings::OTA_UPGRADE, Lang::Strings::UPGRADING, "happy", Lang::Sounds::P3_WELCOME); // 临时用 welcome 替代

            vTaskDelay(pdMS_TO_TICKS(3000));

            SetDeviceState(kDeviceStateUpgrading);

            display->SetIcon(FONT_AWESOME_DOWNLOAD);
            std::string message = std::string(Lang::Strings::NEW_VERSION) + ota.GetFirmwareVersion();
            display->SetChatMessage("system", message.c_str());

            board.SetPowerSaveMode(false);
            audio_service_.Stop();
            vTaskDelay(pdMS_TO_TICKS(1000));

            bool upgrade_success = ota.StartUpgrade([display](int progress, size_t speed)
                                                    { std::thread([display, progress, speed]()
                                                                  {
                    char buffer[32];
                    snprintf(buffer, sizeof(buffer), "%d%% %uKB/s", progress, speed / 1024);
                    display->SetChatMessage("system", buffer); })
                                                          .detach(); });

            if (!upgrade_success)
            {
                ESP_LOGE(TAG, "Firmware upgrade failed, restarting audio service and continuing operation...");
                audio_service_.Start();
                board.SetPowerSaveMode(true);
                Alert(Lang::Strings::ERROR, Lang::Strings::UPGRADE_FAILED, "sad", Lang::Sounds::P3_WELCOME); // 临时用 welcome 替代
                vTaskDelay(pdMS_TO_TICKS(3000));
            }
            else
            {
                ESP_LOGI(TAG, "Firmware upgrade successful, rebooting...");
                display->SetChatMessage("system", "Upgrade successful, rebooting...");
                vTaskDelay(pdMS_TO_TICKS(1000));
                Reboot();
                return;
            }
        }

        ota.MarkCurrentVersionValid();
        if (!ota.HasActivationCode() && !ota.HasActivationChallenge())
        {
            xEventGroupSetBits(event_group_, MAIN_EVENT_CHECK_NEW_VERSION_DONE);
            break;
        }

        display->SetStatus(Lang::Strings::ACTIVATION);
        if (ota.HasActivationCode())
        {
            ShowActivationCode(ota.GetActivationCode(), ota.GetActivationMessage());
        }

        for (int i = 0; i < 10; ++i)
        {
            ESP_LOGI(TAG, "Activating... %d/%d", i + 1, 10);
            esp_err_t err = ota.Activate();
            if (err == ESP_OK)
            {
                xEventGroupSetBits(event_group_, MAIN_EVENT_CHECK_NEW_VERSION_DONE);
                break;
            }
            else if (err == ESP_ERR_TIMEOUT)
            {
                vTaskDelay(pdMS_TO_TICKS(3000));
            }
            else
            {
                vTaskDelay(pdMS_TO_TICKS(10000));
            }
            if (device_state_ == kDeviceStateIdle)
            {
                break;
            }
        }
    }
}

void Application::ShowActivationCode(const std::string &code, const std::string &message)
{
    struct digit_sound
    {
        char digit;
        const std::string_view &sound;
    };
    // 【注意】数字音效使用 0.p3 ~ 9.p3，如果没有这些文件，播放会静默失败
    static const std::array<digit_sound, 10> digit_sounds{{digit_sound{'0', Lang::Sounds::P3_0},
                                                           digit_sound{'1', Lang::Sounds::P3_1},
                                                           digit_sound{'2', Lang::Sounds::P3_2},
                                                           digit_sound{'3', Lang::Sounds::P3_3},
                                                           digit_sound{'4', Lang::Sounds::P3_4},
                                                           digit_sound{'5', Lang::Sounds::P3_5},
                                                           digit_sound{'6', Lang::Sounds::P3_6},
                                                           digit_sound{'7', Lang::Sounds::P3_7},
                                                           digit_sound{'8', Lang::Sounds::P3_8},
                                                           digit_sound{'9', Lang::Sounds::P3_9}}};

    Alert(Lang::Strings::ACTIVATION, message.c_str(), "happy", Lang::Sounds::P3_ACTIVATION);

    for (const auto &digit : code)
    {
        auto it = std::find_if(digit_sounds.begin(), digit_sounds.end(),
                               [digit](const digit_sound &ds)
                               { return ds.digit == digit; });
        if (it != digit_sounds.end())
        {
            audio_service_.PlaySound(it->sound);
        }
    }
}

void Application::Alert(const char *status, const char *message, const char *emotion, const std::string_view &sound)
{
    ESP_LOGW(TAG, "Alert %s: %s [%s]", status, message, emotion);
    auto display = Board::GetInstance().GetDisplay();
    display->SetStatus(status);
    display->SetEmotion(emotion);
    display->SetChatMessage("system", message);
    if (!sound.empty())
    {
        audio_service_.PlaySound(sound);
    }
}

void Application::DismissAlert()
{
    if (device_state_ == kDeviceStateIdle)
    {
        auto display = Board::GetInstance().GetDisplay();
        display->SetStatus(Lang::Strings::STANDBY);
        display->SetEmotion("neutral");
        display->SetChatMessage("system", "");
    }
}

// ========== 以下函数不变，省略中间部分 ==========
// （ToggleChatState, StartListening, StopListening, Start, OnClockTimer, 
//  MainEventLoop, OnWakeWordDetected, AbortSpeaking, SetListeningMode, 
//  SetDeviceState, Reboot, WakeWordInvoke, CanEnterSleepMode, 
//  SendMcpMessage, SetAecMode, AddAudioData, PlaySound 保持不变）

// ⚠️ 注意：下面这些函数中如果有 P3_UPGRADE、P3_EXCLAMATION、P3_POPUP、P3_SUCCESS、P3_VIBRATION 的调用，
// 需要全部替换为 P3_WELCOME 或 P3_ACTIVATION

// 例如在 OnWakeWordDetected 中：
void Application::OnWakeWordDetected()
{
    if (!protocol_)
    {
        return;
    }

    auto& board = Board::GetInstance();
    auto codec = board.GetAudioCodec();
    if (codec) {
        codec->Stop();
        ESP_LOGI(TAG, "Wake word detected: audio stopped");
    }

    if (device_state_ == kDeviceStateIdle)
    {
        audio_service_.EncodeWakeWord();

        if (!protocol_->IsAudioChannelOpened())
        {
            SetDeviceState(kDeviceStateConnecting);
            if (!protocol_->OpenAudioChannel())
            {
                audio_service_.EnableWakeWordDetection(true);
                return;
            }
        }

        auto wake_word = audio_service_.GetLastWakeWord();
        ESP_LOGI(TAG, "Wake word detected: %s", wake_word.c_str());
#if CONFIG_USE_AFE_WAKE_WORD || CONFIG_USE_CUSTOM_WAKE_WORD
        while (auto packet = audio_service_.PopWakeWordPacket())
        {
            protocol_->SendAudio(std::move(packet));
        }
        protocol_->SendWakeWordDetected(wake_word);
        SetListeningMode(aec_mode_ == kAecOff ? kListeningModeAutoStop : kListeningModeRealtime);
#else
        SetListeningMode(aec_mode_ == kAecOff ? kListeningModeAutoStop : kListeningModeRealtime);
        // 【注释】暂时没有 popup.p3
        audio_service_.PlaySound(Lang::Sounds::P3_ACTIVATION); // 临时用 activation 替代
#endif
    }
    else if (device_state_ == kDeviceStateSpeaking)
    {
        ESP_LOGI(TAG, "Wake word interrupt during speaking");
        AbortSpeaking(kAbortReasonWakeWordDetected);
        audio_service_.ResetDecoder();
        SetListeningMode(listening_mode_);
        // 【注释】暂时没有 popup.p3
        audio_service_.PlaySound(Lang::Sounds::P3_ACTIVATION); // 临时用 activation 替代
        ESP_LOGI(TAG, "Wake word interrupt completed, now listening");
    }
    else if (device_state_ == kDeviceStateActivating)
    {
        SetDeviceState(kDeviceStateIdle);
    }
}

// 在 Start() 函数中修改：
// 将 P3_SUCCESS 替换为 P3_WELCOME
// audio_service_.PlaySound(Lang::Sounds::P3_SUCCESS);
// 改为：
audio_service_.PlaySound(Lang::Sounds::P3_WELCOME);

// 在 MainEventLoop 中修改错误提示音：
// Alert(Lang::Strings::ERROR, last_error_message_.c_str(), "sad", Lang::Sounds::P3_EXCLAMATION);
// 改为：
Alert(Lang::Strings::ERROR, last_error_message_.c_str(), "sad", Lang::Sounds::P3_WELCOME);

// 在 protocol_ 的 Alert 回调中：
// Alert(status->valuestring, message->valuestring, emotion->valuestring, Lang::Sounds::P3_VIBRATION);
// 改为（不播放音效）：
Alert(status->valuestring, message->valuestring, emotion->valuestring, {});
