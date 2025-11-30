cmake_minimum_required(VERSION 3.10)
project(ContainersLab VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

configure_file(
    version.h.in
    ${CMAKE_BINARY_DIR}/version.h
)

add_executable(containers_demo main.cpp)

if(WIN32)
    target_compile_definitions(containers_demo PRIVATE OS_WINDOWS)
else()
    target_compile_definitions(containers_demo PRIVATE OS_LINUX)
endif()

enable_testing()
add_test(NAME ContainersTest COMMAND containers_demo)