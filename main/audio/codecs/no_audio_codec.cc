#pragma once

#include "audio_codec.h"
#include <driver/i2s_std.h>
#include <driver/i2s_pdm.h>

class NoAudioCodec : public AudioCodec {
public:
    NoAudioCodec() = default;
    virtual ~NoAudioCodec();

    virtual int Write(const int16_t* data, int samples) override;
    virtual int Read(int16_t* dest, int samples) override;
    
    // 新增：非阻塞读取
    virtual int ReadNonBlocking(int16_t* dest, int samples);
    
    // 新增：输入输出控制
    virtual void EnableInput();
    virtual void DisableInput();
    virtual bool IsInputEnabled() const;
    
    virtual void EnableOutput();
    virtual void DisableOutput();
    virtual bool IsOutputEnabled() const;

protected:
    i2s_chan_handle_t tx_handle_ = nullptr;
    i2s_chan_handle_t rx_handle_ = nullptr;
    bool is_input_enabled_ = false;   // 新增
    bool is_output_enabled_ = false;  // 新增
};

class NoAudioCodecDuplex : public NoAudioCodec {
public:
    NoAudioCodecDuplex(int input_sample_rate, int output_sample_rate, 
                       gpio_num_t bclk, gpio_num_t ws, gpio_num_t dout, gpio_num_t din);
};

class NoAudioCodecSimplex : public NoAudioCodec {
public:
    NoAudioCodecSimplex(int input_sample_rate, int output_sample_rate,
                        gpio_num_t spk_bclk, gpio_num_t spk_ws, gpio_num_t spk_dout,
                        gpio_num_t mic_sck, gpio_num_t mic_ws, gpio_num_t mic_din);
    
    NoAudioCodecSimplex(int input_sample_rate, int output_sample_rate,
                        gpio_num_t spk_bclk, gpio_num_t spk_ws, gpio_num_t spk_dout,
                        i2s_std_slot_mask_t spk_slot_mask,
                        gpio_num_t mic_sck, gpio_num_t mic_ws, gpio_num_t mic_din,
                        i2s_std_slot_mask_t mic_slot_mask);
};

class NoAudioCodecSimplexPdm : public NoAudioCodec {
public:
    NoAudioCodecSimplexPdm(int input_sample_rate, int output_sample_rate,
                           gpio_num_t spk_bclk, gpio_num_t spk_ws, gpio_num_t spk_dout,
                           gpio_num_t mic_sck, gpio_num_t mic_din);
    virtual int Read(int16_t* dest, int samples) override;
};
