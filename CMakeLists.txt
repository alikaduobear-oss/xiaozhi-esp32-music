idf_component_register(
    SRCS
        main.cpp
        app_main.cpp
        # ... 其他源文件
    INCLUDE_DIRS
        "."
        "include"
    EMBED_FILES
        assets/language.json
        assets/th-TH/welcome.p3
        assets/th-TH/active.p3
        assets/th-TH/hello.p3
        assets/th-TH/wifi.p3
        assets/th-TH/language.json
    REQUIRES
        driver
        esp_http_client
        # ... 其他依赖
)
