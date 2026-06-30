// Auto-generated language config
#pragma once

#include <string_view>

namespace Lang {
    // ==========================================
    // 泰语版本（当 CONFIG_LANGUAGE_TH_TH 被定义时）
    // ==========================================
    #ifdef CONFIG_LANGUAGE_TH_TH

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

        // ===== 泰语功能音效 =====
        extern const char p3_active_start[] asm("_binary_active_p3_start");
        extern const char p3_active_end[] asm("_binary_active_p3_end");
        static const std::string_view P3_ACTIVE {
            static_cast<const char*>(p3_active_start),
            static_cast<size_t>(p3_active_end - p3_active_start)
        };

        extern const char p3_hello_start[] asm("_binary_hello_p3_start");
        extern const char p3_hello_end[] asm("_binary_hello_p3_end");
        static const std::string_view P3_HELLO {
            static_cast<const char*>(p3_hello_start),
            static_cast<size_t>(p3_hello_end - p3_hello_start)
        };

        extern const char p3_welcome_start[] asm("_binary_welcome_p3_start");
        extern const char p3_welcome_end[] asm("_binary_welcome_p3_end");
        static const std::string_view P3_WELCOME {
            static_cast<const char*>(p3_welcome_start),
            static_cast<size_t>(p3_welcome_end - p3_welcome_start)
        };

        extern const char p3_wifi_start[] asm("_binary_wifi_p3_start");
        extern const char p3_wifi_end[] asm("_binary_wifi_p3_end");
        static const std::string_view P3_WIFI {
            static_cast<const char*>(p3_wifi_start),
            static_cast<size_t>(p3_wifi_end - p3_wifi_start)
        };

        // ===== 其他音效（如需要，可以继续添加） =====
        // 例如：err_pin, err_reg, success, upgrade, vibration, etc.
    }

    // ==========================================
    // 中文版本（默认，当未定义 CONFIG_LANGUAGE_TH_TH 时）
    // ==========================================
    #else

    constexpr const char* CODE = "zh-CN";

    namespace Strings {
        constexpr const char* ACCESS_VIA_BROWSER = "，浏览器访问 ";
        constexpr const char* ACTIVATION = "激活设备";
        constexpr const char* BATTERY_CHARGING = "正在充电";
        constexpr const char* BATTERY_FULL = "电量已满";
        constexpr const char* BATTERY_LOW = "电量不足";
        constexpr const char* BATTERY_NEED_CHARGE = "电量低，请充电";
        constexpr const char* CHECKING_NEW_VERSION = "检查新版本...";
        constexpr const char* CHECK_NEW_VERSION_FAILED = "检查新版本失败，将在 %d 秒后重试：%s";
        constexpr const char* CONNECTED_TO = "已连接 ";
        constexpr const char* CONNECTING = "连接中...";
        constexpr const char* CONNECT_TO = "连接 ";
        constexpr const char* CONNECT_TO_HOTSPOT = "手机连接热点 ";
        constexpr const char* DETECTING_MODULE = "检测模组...";
        constexpr const char* ENTERING_WIFI_CONFIG_MODE = "进入配网模式...";
        constexpr const char* ERROR = "错误";
        constexpr const char* INFO = "信息";
        constexpr const char* INITIALIZING = "正在初始化...";
        constexpr const char* LISTENING = "聆听中...";
        constexpr const char* LOADING_PROTOCOL = "登录服务器...";
        constexpr const char* MAX_VOLUME = "最大音量";
        constexpr const char* MUTED = "已静音";
        constexpr const char* NEW_VERSION = "新版本 ";
        constexpr const char* OTA_UPGRADE = "OTA 升级";
        constexpr const char* PIN_ERROR = "请插入 SIM 卡";
        constexpr const char* REGISTERING_NETWORK = "等待网络...";
        constexpr const char* REG_ERROR = "无法接入网络，请检查流量卡状态";
        constexpr const char* RTC_MODE_OFF = "AEC 关闭";
        constexpr const char* RTC_MODE_ON = "AEC 开启";
        constexpr const char* SCANNING_WIFI = "扫描 Wi-Fi...";
        constexpr const char* SERVER_ERROR = "发送失败，请检查网络";
        constexpr const char* SERVER_NOT_CONNECTED = "无法连接服务，请稍后再试";
        constexpr const char* SERVER_NOT_FOUND = "正在寻找可用服务";
        constexpr const char* SERVER_TIMEOUT = "等待响应超时";
        constexpr const char* SPEAKING = "说话中...";
        constexpr const char* STANDBY = "待命";
        constexpr const char* SWITCH_TO_4G_NETWORK = "切换到 4G...";
        constexpr const char* SWITCH_TO_WIFI_NETWORK = "切换到 Wi-Fi...";
        constexpr const char* UPGRADE_FAILED = "升级失败";
        constexpr const char* UPGRADING = "正在升级系统...";
        constexpr const char* VERSION = "版本 ";
        constexpr const char* VOLUME = "音量 ";
        constexpr const char* WARNING = "警告";
        constexpr const char* WIFI_CONFIG_MODE = "配网模式";
    }

    namespace Sounds {
        // ===== 中文音效（保持原有，此处仅示意，你需要保留原文件中的完整定义） =====
        // 注意：你需要从原 Lang.h 中复制完整的中文 Sounds 定义到这里
        // 由于你原来的 Lang.h 中已有完整的中文 Sounds，此处只做占位，实际使用时请保留完整的原始内容
        extern const char p3_0_start[] asm("_binary_0_p3_start");
        extern const char p3_0_end[] asm("_binary_0_p3_end");
        static const std::string_view P3_0 {
            static_cast<const char*>(p3_0_start),
            static_cast<size_t>(p3_0_end - p3_0_start)
        };
        // ... 其他音效（请从原文件完整复制）
    }

    #endif
}
