set(SRC "src/main.cpp")
set(APP_NAME restinio_example)

find_package(asio REQUIRED)
find_package(unofficial-http-parser REQUIRED)
find_package(fmt REQUIRED)
find_package(json-dto REQUIRED)
find_package(restinio REQUIRED)

add_executable(${APP_NAME} ${SRC})
target_link_libraries(${APP_NAME} restinio::restinio json-dto::json-dto pthread spdlog::spdlog)
