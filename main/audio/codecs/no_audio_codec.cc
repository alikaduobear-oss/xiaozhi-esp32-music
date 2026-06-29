class NoAudioCodec : public AudioCodec {
private:
    virtual int Write(const int16_t* data, int samples) override;
    virtual int Read(int16_t* dest, int samples) override;

public:
    virtual ~NoAudioCodec();

    // 覆盖基类虚函数
    void EnableInput(bool enable) override;
    void EnableOutput(bool enable) override;   // 新增

    // 额外接口
    int ReadNonBlocking(int16_t* dest, int samples);
    void EnableInput();
    void DisableInput();
    bool IsInputEnabled() const;
};
