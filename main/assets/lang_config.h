// Auto-generated language config
#pragma once

#include <string_view>

#ifndef zh_cn
    #define zh_cn  // 預設語言
#endif

#ifndef th_th
    #define th_th  // 泰语
#endif

namespace Lang {
    // ===== 中文（默认） =====
    #ifdef zh_cn
    constexpr const char* CODE = "zh-CN";

    namespace Strings {
        constexpr const char* ACCESS_VIA_BROWSER = "，浏览器访问 ";
        constexpr const char* ACTIVATION = "激活设备";
        // ... 所有中文字符串（保持原有不变）
    }

    namespace Sounds {
        // ... 所有中文音效（保持原有不变）
    }
    #endif

    // ===== 泰语（新增） =====
    #ifdef th_th
    constexpr const char* CODE = "th-TH";

    namespace Strings {
        constexpr const char* ACCESS_VIA_BROWSER = " เข้าถึงผ่านเบราว์เซอร์ ";
        constexpr const char* ACTIVATION = "เปิดใช้งานอุปกรณ์";
        constexpr const char* BATTERY_CHARGING = "กำลังชาร์จ";
        constexpr const char* BATTERY_FULL = "แบตเตอรี่เต็ม";
        constexpr const char* BATTERY_LOW = "แบตเตอรี่ต่ำ";
        constexpr const char* BATTERY_NEED_CHARGE = "แบตเตอรี่ต่ำ กรุณาชาร์จ";
        constexpr const char* CHECKING_NEW_VERSION = "กำลังตรวจสอบเวอร์ชันใหม่...";
        constexpr const char* CHECK_NEW_VERSION_FAILED = "ตรวจสอบเวอร์ชันใหม่ล้มเหลว จะลองอีกครั้งใน %d วินาที: %s";
        constexpr const char* CONNECTED_TO = "เชื่อมต่อแล้ว ";
        constexpr const char* CONNECTING = "กำลังเชื่อมต่อ...";
        constexpr const char* CONNECT_TO = "เชื่อมต่อ ";
        constexpr const char* CONNECT_TO_HOTSPOT = "เชื่อมต่อฮอตสปอต ";
        constexpr const char* DETECTING_MODULE = "กำลังตรวจจับโมดูล...";
        constexpr const char* ENTERING_WIFI_CONFIG_MODE = "เข้าสู่โหมดตั้งค่า Wi-Fi...";
        constexpr const char* ERROR = "ข้อผิดพลาด";
        constexpr const char* INFO = "ข้อมูล";
        constexpr const char* INITIALIZING = "กำลังเริ่มต้น...";
        constexpr const char* LISTENING = "กำลังฟัง...";
        constexpr const char* LOADING_PROTOCOL = "กำลังเชื่อมต่อเซิร์ฟเวอร์...";
        constexpr const char* MAX_VOLUME = "เสียงสูงสุด";
        constexpr const char* MUTED = "ปิดเสียง";
        constexpr const char* NEW_VERSION = "เวอร์ชันใหม่ ";
        constexpr const char* OTA_UPGRADE = "อัปเกรด OTA";
        constexpr const char* PIN_ERROR = "กรุณาใส่ซิมการ์ด";
        constexpr const char* REGISTERING_NETWORK = "กำลังลงทะเบียนเครือข่าย...";
        constexpr const char* REG_ERROR = "ไม่สามารถเชื่อมต่อเครือข่ายได้ กรุณาตรวจสอบสถานะซิม";
        constexpr const char* RTC_MODE_OFF = "ปิด AEC";
        constexpr const char* RTC_MODE_ON = "เปิด AEC";
        constexpr const char* SCANNING_WIFI = "กำลังสแกน Wi-Fi...";
        constexpr const char* SERVER_ERROR = "ส่งข้อความล้มเหลว กรุณาตรวจสอบเครือข่าย";
        constexpr const char* SERVER_NOT_CONNECTED = "ไม่สามารถเชื่อมต่อเซิร์ฟเวอร์ได้ กรุณาลองอีกครั้ง";
        constexpr const char* SERVER_NOT_FOUND = "กำลังค้นหาเซิร์ฟเวอร์ที่ใช้งานได้";
        constexpr const char* SERVER_TIMEOUT = "หมดเวลารอการตอบสนอง";
        constexpr const char* SPEAKING = "กำลังพูด...";
        constexpr const char* STANDBY = "พร้อมใช้งาน";
        constexpr const char* SWITCH_TO_4G_NETWORK = "สลับไปที่ 4G...";
        constexpr const char* SWITCH_TO_WIFI_NETWORK = "สลับไปที่ Wi-Fi...";
        constexpr const char* UPGRADE_FAILED = "อัปเกรดล้มเหลว";
        constexpr const char* UPGRADING = "กำลังอัปเกรดระบบ...";
        constexpr const char* VERSION = "เวอร์ชัน ";
        constexpr const char* VOLUME = "ระดับเสียง ";
        constexpr const char* WARNING = "คำเตือน";
        constexpr const char* WIFI_CONFIG_MODE = "โหมดตั้งค่า Wi-Fi";
    }

    namespace Sounds {
        // ===== 数字 0-9 =====
        extern const char p3_0_start[] asm("_binary_0_p3_start");
        extern const char p3_0_end[] asm("_binary_0_p3_end");
        static const std::string_view P3_0 {
            static_cast<const char*>(p3_0_start),
            static_cast<size_t>(p3_0_end - p3_0_start)
        };

        extern const char p3_1_start[] asm("_binary_1_p3_start");
        extern const char p3_1_end[] asm("_binary_1_p3_end");
        static const std::string_view P3_1 {
            static_cast<const char*>(p3_1_start),
            static_cast<size_t>(p3_1_end - p3_1_start)
        };

        extern const char p3_2_start[] asm("_binary_2_p3_start");
        extern const char p3_2_end[] asm("_binary_2_p3_end");
        static const std::string_view P3_2 {
            static_cast<const char*>(p3_2_start),
            static_cast<size_t>(p3_2_end - p3_2_start)
        };

        extern const char p3_3_start[] asm("_binary_3_p3_start");
        extern const char p3_3_end[] asm("_binary_3_p3_end");
        static const std::string_view P3_3 {
            static_cast<const char*>(p3_3_start),
            static_cast<size_t>(p3_3_end - p3_3_start)
        };

        extern const char p3_4_start[] asm("_binary_4_p3_start");
        extern const char p3_4_end[] asm("_binary_4_p3_end");
        static const std::string_view P3_4 {
            static_cast<const char*>(p3_4_start),
            static_cast<size_t>(p3_4_end - p3_4_start)
        };

        extern const char p3_5_start[] asm("_binary_5_p3_start");
        extern const char p3_5_end[] asm("_binary_5_p3_end");
        static const std::string_view P3_5 {
            static_cast<const char*>(p3_5_start),
            static_cast<size_t>(p3_5_end - p3_5_start)
        };

        extern const char p3_6_start[] asm("_binary_6_p3_start");
        extern const char p3_6_end[] asm("_binary_6_p3_end");
        static const std::string_view P3_6 {
            static_cast<const char*>(p3_6_start),
            static_cast<size_t>(p3_6_end - p3_6_start)
        };

        extern const char p3_7_start[] asm("_binary_7_p3_start");
        extern const char p3_7_end[] asm("_binary_7_p3_end");
        static const std::string_view P3_7 {
            static_cast<const char*>(p3_7_start),
            static_cast<size_t>(p3_7_end - p3_7_start)
        };

        extern const char p3_8_start[] asm("_binary_8_p3_start");
        extern const char p3_8_end[] asm("_binary_8_p3_end");
        static const std::string_view P3_8 {
            static_cast<const char*>(p3_8_start),
            static_cast<size_t>(p3_8_end - p3_8_start)
        };

        extern const char p3_9_start[] asm("_binary_9_p3_start");
        extern const char p3_9_end[] asm("_binary_9_p3_end");
        static const std::string_view P3_9 {
            static_cast<const char*>(p3_9_start),
            static_cast<size_t>(p3_9_end - p3_9_start)
        };

        // ===== 功能音效（泰语版本） =====
        // active.p3 → 激活提示音
        extern const char p3_active_start[] asm("_binary_active_p3_start");
        extern const char p3_active_end[] asm("_binary_active_p3_end");
        static const std::string_view P3_ACTIVE {
            static_cast<const char*>(p3_active_start),
            static_cast<size_t>(p3_active_end - p3_active_start)
        };

        // hello.p3 → 问候音
        extern const char p3_hello_start[] asm("_binary_hello_p3_start");
        extern const char p3_hello_end[] asm("_binary_hello_p3_end");
        static const std::string_view P3_HELLO {
            static_cast<const char*>(p3_hello_start),
            static_cast<size_t>(p3_hello_end - p3_hello_start)
        };

        // welcome.p3 → 欢迎音
        extern const char p3_welcome_start[] asm("_binary_welcome_p3_start");
        extern const char p3_welcome_end[] asm("_binary_welcome_p3_end");
        static const std::string_view P3_WELCOME {
            static_cast<const char*>(p3_welcome_start),
            static_cast<size_t>(p3_welcome_end - p3_welcome_start)
        };

        // wifi.p3 → Wi-Fi 配置提示音
        extern const char p3_wifi_start[] asm("_binary_wifi_p3_start");
        extern const char p3_wifi_end[] asm("_binary_wifi_p3_end");
        static const std::string_view P3_WIFI {
            static_cast<const char*>(p3_wifi_start),
            static_cast<size_t>(p3_wifi_end - p3_wifi_start)
        };
    }
    #endif
}
