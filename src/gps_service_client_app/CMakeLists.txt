set(SRC "src/main.cpp")
set(APP_NAME gps_service_client_app)

add_executable(${APP_NAME} ${SRC})
target_link_libraries(${APP_NAME} gps_service_client_lib)
