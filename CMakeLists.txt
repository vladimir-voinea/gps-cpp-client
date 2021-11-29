cmake_minimum_required(VERSION 3.5)
project(CppGrpcTemplate)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
include(/vcpkg/scripts/buildsystems/vcpkg.cmake)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(spdlog REQUIRED)

set(CMAKE_CXX_STANDARD 17)

enable_testing()

add_subdirectory(src)
