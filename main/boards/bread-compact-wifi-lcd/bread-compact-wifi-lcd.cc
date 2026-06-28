else if (device_state_ == kDeviceStateSpeaking)
{
    ESP_LOGI(TAG, "Wake word interrupt during speaking");
    AbortSpeaking(kAbortReasonWakeWordDetected);
    audio_service_.ResetDecoder();
    SetListeningMode(listening_mode_);
    audio_service_.PlaySound(Lang::Sounds::P3_POPUP);
    ESP_LOGI(TAG, "Wake word interrupt completed, now listening");
}
