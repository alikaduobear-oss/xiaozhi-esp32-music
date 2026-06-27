#include "wifi_board.h"
#include "codecs/no_audio_codec.h"
#include "display/lcd_display.h"
#include "system_reset.h"
#include "application.h"
#include "button.h"
#include "config.h"
#include "mcp_server.h"
#include "lamp_controller.h"
#include "led/single_led.h"

#include <wifi_station.h>
#include <esp_log.h>
#include <driver/i2c_master.h>
#include <esp_lcd_panel_vendor.h>
#include <esp_lcd_panel_io.h>
#include <esp_lcd_panel_ops.h>
#include <driver/spi_common.h>

// ST7735 驱动头文件
#include "esp_lcd_st7735.h"

#define TAG "CompactWifiBoardLCD"

LV_FONT_DECLARE(font_puhui_16_4);
LV_FONT_DECLARE(font_awesome_16_4);

// ============================================
// 硬件引脚定义 - 根据你的接线修改
// ============================================

// 屏幕引脚 (ST7735, 128x160, SPI)
#define DISPLAY_MOSI_PIN    GPIO_NUM_11     // SDA
#define DISPLAY_CLK_PIN     GPIO_NUM_12     // SCL
#define DISPLAY_RST_PIN     GPIO_NUM_5      // RST
#define DISPLAY_DC_PIN      GPIO_NUM_4      // DC
#define DISPLAY_CS_PIN      GPIO_NUM_6      // CS
#define DISPLAY_BACKLIGHT_PIN GPIO_NUM_7    // BLK

// 屏幕参数
#define DISPLAY_WIDTH       128
#define DISPLAY_HEIGHT      160
#define DISPLAY_OFFSET_X    0
#define DISPLAY_OFFSET_Y    0
#define DISPLAY_MIRROR_X    false
#define DISPLAY_MIRROR_Y    false
#define DISPLAY_SWAP_XY     false
#define DISPLAY_INVERT_COLOR true           // ST7735 通常需要颜色反转
#define DISPLAY_RGB_ORDER   LCD_RGB_ELEMENT_ORDER_RGB
#define DISPLAY_SPI_MODE    0

// 音频引脚 - Simplex 模式（输入输出分开）
// 功放 MAX98357 (输出)
#define AUDIO_I2S_SPK_GPIO_BCLK     GPIO_NUM_13     // BCLK
#define AUDIO_I2S_SPK_GPIO_LRCK     GPIO_NUM_14     // LRCK (WS)
#define AUDIO_I2S_SPK_GPIO_DOUT     GPIO_NUM_15     // DIN

// 麦克风 MS4030 (输入)
#define AUDIO_I2S_MIC_GPIO_SCK      GPIO_NUM_9      // SCK (BCLK)
#define AUDIO_I2S_MIC_GPIO_WS       GPIO_NUM_8      // WS (LRCK)
#define AUDIO_I2S_MIC_GPIO_DIN      GPIO_NUM_10     // SD (数据输出到ESP32)

// 采样率
#define AUDIO_INPUT_SAMPLE_RATE     16000
#define AUDIO_OUTPUT_SAMPLE_RATE    16000

// 其他引脚
#define BOOT_BUTTON_GPIO    GPIO_NUM_0      // 启动/配置按钮（通常用GPIO0）
#define BUILTIN_LED_GPIO    GPIO_NUM_48     // 板载LED（根据实际修改）
#define LAMP_GPIO           GPIO_NUM_47     // 灯控制（根据实际修改）

// ============================================

class CompactWifiBoardLCD : public WifiBoard {
private:
    Button boot_button_;
    LcdDisplay* display_;

    void InitializeSpi() {
        spi_bus_config_t buscfg = {};
        buscfg.mosi_io_num = DISPLAY_MOSI_PIN;
        buscfg.miso_io_num = GPIO_NUM_NC;       // 屏幕无 MISO
        buscfg.sclk_io_num = DISPLAY_CLK_PIN;
        buscfg.quadwp_io_num = GPIO_NUM_NC;
        buscfg.quadhd_io_num = GPIO_NUM_NC;
        buscfg.max_transfer_sz = DISPLAY_WIDTH * DISPLAY_HEIGHT * sizeof(uint16_t);
        ESP_ERROR_CHECK(spi_bus_initialize(SPI3_HOST, &buscfg, SPI_DMA_CH_AUTO));
    }

    void InitializeLcdDisplay() {
        esp_lcd_panel_io_handle_t panel_io = nullptr;
        esp_lcd_panel_handle_t panel = nullptr;

        // 液晶屏控制IO初始化
        ESP_LOGD(TAG, "Install panel IO");
        esp_lcd_panel_io_spi_config_t io_config = {};
        io_config.cs_gpio_num = DISPLAY_CS_PIN;
        io_config.dc_gpio_num = DISPLAY_DC_PIN;
        io_config.spi_mode = DISPLAY_SPI_MODE;
        io_config.pclk_hz = 40 * 1000 * 1000;   // 40MHz SPI时钟
        io_config.trans_queue_depth = 10;
        io_config.lcd_cmd_bits = 8;
        io_config.lcd_param_bits = 8;
        ESP_ERROR_CHECK(esp_lcd_new_panel_io_spi(SPI3_HOST, &io_config, &panel_io));

        // 初始化 ST7735 液晶屏驱动芯片
        ESP_LOGD(TAG, "Install ST7735 LCD driver");
        esp_lcd_panel_dev_config_t panel_config = {};
        panel_config.reset_gpio_num = DISPLAY_RST_PIN;
        panel_config.rgb_ele_order = DISPLAY_RGB_ORDER;
        panel_config.bits_per_pixel = 16;
        
        ESP_ERROR_CHECK(esp_lcd_new_panel_st7735(panel_io, &panel_config, &panel));

        // 复位并初始化屏幕
        esp_lcd_panel_reset(panel);
        esp_lcd_panel_init(panel);
        
        // ST7735 配置
        esp_lcd_panel_invert_color(panel, DISPLAY_INVERT_COLOR);
        esp_lcd_panel_swap_xy(panel, DISPLAY_SWAP_XY);
        esp_lcd_panel_mirror(panel, DISPLAY_MIRROR_X, DISPLAY_MIRROR_Y);

        // 创建显示对象
        display_ = new SpiLcdDisplay(panel_io, panel,
                                    DISPLAY_WIDTH, DISPLAY_HEIGHT, 
                                    DISPLAY_OFFSET_X, DISPLAY_OFFSET_Y, 
                                    DISPLAY_MIRROR_X, DISPLAY_MIRROR_Y, 
                                    DISPLAY_SWAP_XY,
                                    {
                                        .text_font = &font_puhui_16_4,
                                        .icon_font = &font_awesome_16_4,
#if CONFIG_USE_WECHAT_MESSAGE_STYLE
                                        .emoji_font = font_emoji_32_init(),
#else
                                        .emoji_font = DISPLAY_HEIGHT >= 240 ? font_emoji_64_init() : font_emoji_32_init(),
#endif
                                    });
    }

    void InitializeButtons() {
        boot_button_.OnClick([this]() {
            auto& app = Application::GetInstance();
            if (app.GetDeviceState() == kDeviceStateStarting && !WifiStation::GetInstance().IsConnected()) {
                ResetWifiConfiguration();
            }
            app.ToggleChatState();
        });
    }

    // 物联网初始化，添加对 AI 可见设备
    void InitializeTools() {
        static LampController lamp(LAMP_GPIO);
    }

public:
    CompactWifiBoardLCD() :
        boot_button_(BOOT_BUTTON_GPIO) {
        InitializeSpi();
        InitializeLcdDisplay();
        InitializeButtons();
        InitializeTools();
        
        if (DISPLAY_BACKLIGHT_PIN != GPIO_NUM_NC) {
            GetBacklight()->RestoreBrightness();
        }
    }

    virtual Led* GetLed() override {
        static SingleLed led(BUILTIN_LED_GPIO);
        return &led;
    }

    virtual AudioCodec* GetAudioCodec() override {
        // 使用 Simplex 模式：麦克风和功放使用不同的 I2S 引脚
        static NoAudioCodecSimplex audio_codec(
            AUDIO_INPUT_SAMPLE_RATE, 
            AUDIO_OUTPUT_SAMPLE_RATE,
            AUDIO_I2S_SPK_GPIO_BCLK,    // 功放 BCLK
            AUDIO_I2S_SPK_GPIO_LRCK,    // 功放 LRCK
            AUDIO_I2S_SPK_GPIO_DOUT,    // 功放 DIN
            AUDIO_I2S_MIC_GPIO_SCK,     // 麦克风 SCK
            AUDIO_I2S_MIC_GPIO_WS,      // 麦克风 WS
            AUDIO_I2S_MIC_GPIO_DIN      // 麦克风 DIN
        );
        return &audio_codec;
    }

    virtual Display* GetDisplay() override {
        return display_;
    }

    virtual Backlight* GetBacklight() override {
        if (DISPLAY_BACKLIGHT_PIN != GPIO_NUM_NC) {
            static PwmBacklight backlight(DISPLAY_BACKLIGHT_PIN, false);  // 非反转
            return &backlight;
        }
        return nullptr;
    }
};

DECLARE_BOARD(CompactWifiBoardLCD);
