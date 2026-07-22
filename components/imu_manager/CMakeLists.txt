idf_component_register(
    SRCS
        "imu_manager.cpp"

    INCLUDE_DIRS
        "."

    REQUIRES
        waveshare__qmi8658
        esp32_s3_touch_amoled_2_06
)