#ifndef _NO_AUDIO_CODEC_H
#define _NO_AUDIO_CODEC_H

#include "audio_codec.h"
#include <driver/gpio.h>
#include <driver/i2s_pdm.h>

class NoAudioCodec : public AudioCodec {
private:
    virtual int Write(const int16_t* data, int samples) override;
    virtual int Read(int16_t* dest, int samples) override;

public:
    virtual ~NoAudioCodec();

    // 覆盖基类的虚函数
    void EnableInput(bool enable) override;
    void EnableOutput(bool enable) override;   // 关键：必须存在

    // 额外接口
    int ReadNonBlocking(int16_t* dest, int samples);
    void EnableInput();
    void DisableInput();
    bool IsInputEnabled() const;
};

// 子类定义（保持不变）
class NoAudioCodecDuplex : public NoAudioCodec {
public:
    NoAudioCodecDuplex(int input_sample_rate, int output_sample_rate,
                       gpio_num_t bclk, gpio_num_t ws,
                       gpio_num_t dout, gpio_num_t din);
};

class NoAudioCodecSimplex : public NoAudioCodec {
public:
    NoAudioCodecSimplex(int input_sample_rate, int output_sample_rate,
                        gpio_num_t spk_bclk, gpio_num_t spk_ws,
                        gpio_num_t spk_dout, gpio_num_t mic_sck,
                        gpio_num_t mic_ws, gpio_num_t mic_din);
    NoAudioCodecSimplex(int input_sample_rate, int output_sample_rate,
                        gpio_num_t spk_bclk, gpio_num_t spk_ws,
                        gpio_num_t spk_dout, i2s_std_slot_mask_t spk_slot_mask,
                        gpio_num_t mic_sck, gpio_num_t mic_ws,
                        gpio_num_t mic_din, i2s_std_slot_mask_t mic_slot_mask);
};

class NoAudioCodecSimplexPdm : public NoAudioCodec {
public:
    NoAudioCodecSimplexPdm(int input_sample_rate, int output_sample_rate,
                           gpio_num_t spk_bclk, gpio_num_t spk_ws,
                           gpio_num_t spk_dout, gpio_num_t mic_sck,
                           gpio_num_t mic_din);
    int Read(int16_t* dest, int samples) override;
};

#endif // _NO_AUDIO_CODEC_H
