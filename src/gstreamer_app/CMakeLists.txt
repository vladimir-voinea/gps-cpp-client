set(SRC "src/main.cpp")
set(APP_NAME gstreamer_app)

find_package(PkgConfig) 
pkg_search_module(GST REQUIRED gstreamer-1.0>=1.4
    gstreamer-sdp-1.0>=1.4
    gstreamer-video-1.0>=1.4
    gstreamer-app-1.0>=1.4)

add_executable(${APP_NAME} ${SRC})

target_include_directories(${APP_NAME} PRIVATE ${GST_INCLUDE_DIRS})
target_link_libraries(${APP_NAME} ${GST_LIBRARIES} spdlog::spdlog)
