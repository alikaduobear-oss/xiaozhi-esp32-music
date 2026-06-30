idf_component_register(
    SRCS
        # 主程序源文件（根据你的项目实际情况添加）
        main.cpp
        app_main.cpp
        # 其他源文件...
    INCLUDE_DIRS
        "."
        "include"
    EMBED_FILES
        # 语言配置文件（如果存在）
        assets/language.json
        
        # 泰语本地化文件（路径：main/assets/th-TH/）
        assets/th-TH/welcome.p3
        assets/th-TH/active.p3
        assets/th-TH/hello.p3
        assets/th-TH/wifi.p3
        assets/th-TH/language.json
        
        # 如果还有数字或其他音效，继续添加
        # assets/th-TH/0.p3
        # assets/th-TH/1.p3
        # assets/th-TH/2.p3
        # ...
    
    REQUIRES
        driver
        esp_http_client
        esp_https_ota
        esp_timer
        freertos
        json
        lvgl
        mbedtls
        nvs_flash
        spi_flash
        wifi
        # 添加你的项目需要的其他组件
)
