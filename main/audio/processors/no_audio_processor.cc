#include "no_audio_processor.h"
#include <esp_log.h>

#define TAG "NoAudioProcessor"

void NoAudioProcessor::Initialize(AudioCodec* codec, int frame_duration_ms) {
    codec_ = codec;
    frame_samples_ = frame_duration_ms * 16000 / 1000;
}

void NoAudioProcessor::Feed(std::vector<int16_t>&& data) {
    if (!is_running_ || !output_callback_) {
        return;
    }

    if (data.size() != frame_samples_) {
        ESP_LOGE(TAG, "Feed data size is not equal to frame size, feed size: %u, frame size: %u", data.size(), frame_samples_);
        return;
    }

    // 保存原始数据用于唤醒词检测（在发送给输出回调前）
    std::vector<int16_t> original_data;
    if (wake_word_detect_callback_) {
        original_data = data;  // 复制一份用于唤醒词检测
    }

    if (codec_->input_channels() == 2) {
        // If input channels is 2, we need to fetch the left channel data
        auto mono_data = std::vector<int16_t>(data.size() / 2);
        for (size_t i = 0, j = 0; i < mono_data.size(); ++i, j += 2) {
            mono_data[i] = data[j];
        }
        output_callback_(std::move(mono_data));
    } else {
        output_callback_(std::move(data));
    }

    // 唤醒词检测：即使正在播放，也持续检测
    if (wake_word_detect_callback_ && !original_data.empty()) {
        // 如果是双声道，提取单声道数据用于唤醒词检测
        std::vector<int16_t> mono_data_for_wake;
        if (codec_->input_channels() == 2) {
            mono_data_for_wake.resize(original_data.size() / 2);
            for (size_t i = 0, j = 0; i < mono_data_for_wake.size(); ++i, j += 2) {
                mono_data_for_wake[i] = original_data[j];
            }
        } else {
            mono_data_for_wake = std::move(original_data);
        }
        
        // 调用唤醒词检测
        wake_word_detect_callback_(std::move(mono_data_for_wake));
    }
}

void NoAudioProcessor::Start() {
    is_running_ = true;
}

void NoAudioProcessor::Stop() {
    is_running_ = false;
}

bool NoAudioProcessor::IsRunning() {
    return is_running_;
}

void NoAudioProcessor::OnOutput(std::function<void(std::vector<int16_t>&& data)> callback) {
    output_callback_ = callback;
}

void NoAudioProcessor::OnVadStateChange(std::function<void(bool speaking)> callback) {
    vad_state_change_callback_ = callback;
}

// 新增：设置唤醒词检测回调
void NoAudioProcessor::OnWakeWordDetect(std::function<void(std::vector<int16_t>&& data)> callback) {
    wake_word_detect_callback_ = callback;
}

size_t NoAudioProcessor::GetFeedSize() {
    if (!codec_) {
        return 0;
    }
    return frame_samples_;
}

void NoAudioProcessor::EnableDeviceAec(bool enable) {
    if (enable) {
        ESP_LOGE(TAG, "Device AEC is not supported");
    }
}
