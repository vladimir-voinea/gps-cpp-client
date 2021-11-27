cmake_minimum_required(VERSION 3.5)
project(CppGrpcTemplate)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

set(CMAKE_CXX_STANDARD 17)

enable_testing()

add_subdirectory(src)
