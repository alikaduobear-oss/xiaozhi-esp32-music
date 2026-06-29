#include "audio_codec.h"
#include "board.h"
#include "settings.h"

#include <esp_log.h>
#include <cstring>
#include <driver/i2s_common.h>

#define TAG "AudioCodec"

AudioCodec::AudioCodec() {
}

AudioCodec::~AudioCodec() {
}

void AudioCodec::OutputData(std::vector<int16_t>& data) {
    Write(data.data(), data.size());
}

bool AudioCodec::InputData(std::vector<int16_t>& data) {
    int samples = Read(data.data(), data.size());
    if (samples > 0) {
        return true;
    }
    return false;
}

void AudioCodec::Start() {
    Settings settings("audio", false);
    output_volume_ = settings.GetInt("output_volume", output_volume_);
    if (output_volume_ <= 0) {
        ESP_LOGW(TAG, "Output volume value (%d) is too small, setting to default (10)", output_volume_);
        output_volume_ = 10;
    }

    if (original_output_sample_rate_ == 0) {
        original_output_sample_rate_ = output_sample_rate_;
        ESP_LOGI(TAG, "Saved original output sample rate: %d Hz", original_output_sample_rate_);
    }

    // 启用输出和输入（通过调用 EnableOutput/EnableInput，它们会操作硬件）
    EnableOutput(true);
    EnableInput(true);

    ESP_LOGI(TAG, "Audio codec started, output_enabled=%d, input_enabled=%d",
             output_enabled_, input_enabled_);
}

void AudioCodec::SetOutputVolume(int volume) {
    output_volume_ = volume;
    ESP_LOGI(TAG, "Set output volume to %d", output_volume_);
    
    Settings settings("audio", true);
    settings.SetInt("output_volume", output_volume_);
}

void AudioCodec::EnableInput(bool enable) {
    if (enable == input_enabled_) {
        return;
    }

    esp_err_t err;
    if (enable) {
        if (rx_handle_ != nullptr) {
            err = i2s_channel_enable(rx_handle_);
            if (err == ESP_OK) {
                input_enabled_ = true;
                ESP_LOGI(TAG, "Input enabled");
            } else {
                ESP_LOGE(TAG, "Failed to enable input: %s", esp_err_to_name(err));
            }
        } else {
            ESP_LOGW(TAG, "rx_handle_ is null");
        }
    } else {
        if (rx_handle_ != nullptr) {
            err = i2s_channel_disable(rx_handle_);
            if (err == ESP_OK || err == ESP_ERR_INVALID_STATE) {
                input_enabled_ = false;
                ESP_LOGI(TAG, "Input disabled");
            } else {
                ESP_LOGE(TAG, "Failed to disable input: %s", esp_err_to_name(err));
            }
        } else {
            input_enabled_ = false;
        }
    }
}

void AudioCodec::EnableOutput(bool enable) {
    if (enable == output_enabled_) {
        return;
    }

    esp_err_t err;
    if (enable) {
        if (tx_handle_ != nullptr) {
            err = i2s_channel_enable(tx_handle_);
            if (err == ESP_OK) {
                output_enabled_ = true;
                ESP_LOGI(TAG, "Output enabled");
            } else {
                ESP_LOGE(TAG, "Failed to enable output: %s", esp_err_to_name(err));
            }
        } else {
            ESP_LOGE(TAG, "tx_handle_ is NULL");
        }
    } else {
        if (tx_handle_ != nullptr) {
            err = i2s_channel_disable(tx_handle_);
            if (err == ESP_OK || err == ESP_ERR_INVALID_STATE) {
                output_enabled_ = false;
                ESP_LOGI(TAG, "Output disabled");
            } else {
                ESP_LOGE(TAG, "Failed to disable output: %s", esp_err_to_name(err));
            }
        } else {
            output_enabled_ = false;
        }
    }
}

bool AudioCodec::SetOutputSampleRate(int sample_rate) {
    if (sample_rate == -1) {
        if (original_output_sample_rate_ > 0) {
            sample_rate = original_output_sample_rate_;
            ESP_LOGI(TAG, "Resetting to original output sample rate: %d Hz", sample_rate);
        } else {
            ESP_LOGW(TAG, "Original sample rate not available");
            return false;
        }
    }
    
    if (sample_rate <= 0 || sample_rate > 192000) {
        ESP_LOGE(TAG, "Invalid sample rate: %d", sample_rate);
        return false;
    }
    
    if (output_sample_rate_ == sample_rate) {
        ESP_LOGI(TAG, "Sample rate already set to %d Hz", sample_rate);
        return true;
    }
    
    if (tx_handle_ == nullptr) {
        ESP_LOGW(TAG, "TX handle is null, only updating variable");
        output_sample_rate_ = sample_rate;
        return true;
    }
    
    ESP_LOGI(TAG, "Changing output sample rate from %d to %d Hz", output_sample_rate_, sample_rate);
    
    bool was_enabled = output_enabled_;
    if (was_enabled) {
        esp_err_t disable_ret = i2s_channel_disable(tx_handle_);
        if (disable_ret == ESP_OK || disable_ret == ESP_ERR_INVALID_STATE) {
            ESP_LOGI(TAG, "I2S TX disabled for reconfiguration");
        } else {
            ESP_LOGW(TAG, "Failed to disable I2S TX: %s", esp_err_to_name(disable_ret));
        }
        output_enabled_ = false;
    }
    
    i2s_std_clk_config_t clk_cfg = {
        .sample_rate_hz = (uint32_t)sample_rate,
        .clk_src = I2S_CLK_SRC_DEFAULT,
        .mclk_multiple = I2S_MCLK_MULTIPLE_256,
#ifdef I2S_HW_VERSION_2
        .ext_clk_freq_hz = 0,
#endif
    };
    
    esp_err_t ret = i2s_channel_reconfig_std_clock(tx_handle_, &clk_cfg);
    
    if (was_enabled) {
        esp_err_t enable_ret = i2s_channel_enable(tx_handle_);
        if (enable_ret == ESP_OK) {
            output_enabled_ = true;
            ESP_LOGI(TAG, "I2S TX re-enabled");
        } else {
            ESP_LOGE(TAG, "Failed to re-enable I2S TX: %s", esp_err_to_name(enable_ret));
        }
    }
    
    if (ret == ESP_OK) {
        output_sample_rate_ = sample_rate;
        ESP_LOGI(TAG, "Successfully changed output sample rate to %d Hz", sample_rate);
        return true;
    } else {
        ESP_LOGE(TAG, "Failed to change sample rate: %s", esp_err_to_name(ret));
        return false;
    }
}

void AudioCodec::Stop() {
    ESP_LOGI(TAG, "Stop requested, disabling output");
    if (tx_handle_ != nullptr) {
        esp_err_t err = i2s_channel_disable(tx_handle_);
        if (err == ESP_OK || err == ESP_ERR_INVALID_STATE) {
            output_enabled_ = false;
            ESP_LOGI(TAG, "Output disabled");
        } else {
            ESP_LOGW(TAG, "Failed to disable output: %s", esp_err_to_name(err));
        }
    } else {
        output_enabled_ = false;
    }
}
