set(SRCS src/gps_service_client.cpp)
set(LIB_NAME gps_service_client_lib)

add_library(${LIB_NAME} ${SRCS})
target_include_directories(${LIB_NAME} PUBLIC include)
target_link_libraries(${LIB_NAME} PUBLIC proto-gen spdlog::spdlog)