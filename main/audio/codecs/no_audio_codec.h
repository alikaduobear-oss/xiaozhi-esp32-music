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

    // 覆盖基类的带参 EnableInput (外部调用需要)
    void EnableInput(bool enable) override;

    // 额外的接口（可保留）
    int ReadNonBlocking(int16_t* dest, int samples);
    void EnableInput();          // 无参版本（兼容旧代码）
    void DisableInput();
    bool IsInputEnabled() const;
};

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
    // 注意：此处公有覆盖基类 private 虚函数，可能引发警告，但通常不影响运行
    int Read(int16_t* dest, int samples) override;
};

#endif // _NO_AUDIO_CODEC_H
