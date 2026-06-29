#include "no_audio_codec.h"

#include <esp_log.h>
#include <cmath>
#include <cstring>
#include <cstdint>

#define TAG "NoAudioCodec"

// 析构函数
NoAudioCodec::~NoAudioCodec() {
    if (rx_handle_ != nullptr) {
        ESP_ERROR_CHECK(i2s_channel_disable(rx_handle_));
    }
    if (tx_handle_ != nullptr) {
        ESP_ERROR_CHECK(i2s_channel_disable(tx_handle_));
    }
}

// 构造函数（Duplex, Simplex, SimplexPdm）保持原样，此处省略...

// ==================== Write 函数 ====================
int NoAudioCodec::Write(const int16_t* data, int samples) {
    if (!output_enabled_ && tx_handle_ != nullptr) {
        ESP_LOGI(TAG, "Output disabled, re-enabling via EnableOutput");
        EnableOutput(true);   // 调用本类的 EnableOutput
    }

    if (tx_handle_ == nullptr || !output_enabled_) {
        ESP_LOGE(TAG, "Output channel not available");
        return 0;
    }

    std::vector<int32_t> buffer(samples);
    int32_t volume_factor = pow(double(output_volume_) / 100.0, 2) * 65536;
    for (int i = 0; i < samples; i++) {
        int64_t temp = int64_t(data[i]) * volume_factor;
        if (temp > INT32_MAX) buffer[i] = INT32_MAX;
        else if (temp < INT32_MIN) buffer[i] = INT32_MIN;
        else buffer[i] = static_cast<int32_t>(temp);
    }

    size_t bytes_written;
    esp_err_t err = i2s_channel_write(tx_handle_, buffer.data(),
                                      samples * sizeof(int32_t),
                                      &bytes_written, portMAX_DELAY);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Write failed: %s", esp_err_to_name(err));
        return 0;
    }
    return bytes_written / sizeof(int32_t);
}

// Read, ReadNonBlocking 等保持不变...

// ==================== EnableInput(bool) ====================
void NoAudioCodec::EnableInput(bool enable) {
    if (enable) {
        if (rx_handle_ != nullptr && !input_enabled_) {
            ESP_ERROR_CHECK(i2s_channel_enable(rx_handle_));
            input_enabled_ = true;
            ESP_LOGI(TAG, "Input enabled (with param)");
        }
    } else {
        if (rx_handle_ != nullptr && input_enabled_) {
            ESP_ERROR_CHECK(i2s_channel_disable(rx_handle_));
            input_enabled_ = false;
            ESP_LOGI(TAG, "Input disabled (with param)");
        }
    }
}

// ==================== EnableOutput(bool) ====================
void NoAudioCodec::EnableOutput(bool enable) {
    // 调用基类实现，基类会操作硬件并更新标志
    AudioCodec::EnableOutput(enable);
}

// 无参 EnableInput/DisableInput/IsInputEnabled 保留...
