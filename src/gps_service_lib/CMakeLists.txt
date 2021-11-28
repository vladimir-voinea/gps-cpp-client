set(SRCS src/gps_service.cpp src/gpsd.cpp)
set(LIB_NAME gps_service_lib)

find_package(GPS REQUIRED)

add_library(${LIB_NAME} ${SRCS})
target_include_directories(${LIB_NAME} PUBLIC include)
target_link_libraries(${LIB_NAME} PUBLIC proto-gen spdlog::spdlog GPS::GPS)
