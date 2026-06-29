#ifndef WIFI_BOARD_H
#define WIFI_BOARD_H

#include "board.h"

class WifiBoard : public Board {
protected:
    bool wifi_config_mode_ = false;
    void EnterWifiConfigMode();
    virtual std::string GetBoardJson() override;

public:
    WifiBoard();
    virtual std::string GetBoardType() override;
    virtual void StartNetwork() override;
    virtual NetworkInterface* GetNetwork() override;
    virtual const char* GetNetworkStateIcon() override;
    virtual void SetPowerSaveMode(bool enabled) override;
    virtual void ResetWifiConfiguration();
    virtual AudioCodec* GetAudioCodec() override { return nullptr; }
    virtual std::string GetDeviceStatusJson() override;

    // ===== 新增：唤醒词检测回调接口 =====
    // 子类可覆盖此方法以处理唤醒词事件（如停止播放、切换状态等）
    virtual void OnWakeWordDetected() {}
};

#endif // WIFI_BOARD_H
