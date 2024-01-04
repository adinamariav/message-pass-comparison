cmake_minimum_required(VERSION 3.13)
project(message-pass-comparison)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory("${PROJECT_SOURCE_DIR}/client")
add_subdirectory("${PROJECT_SOURCE_DIR}/server")

install(TARGETS client server
        DESTINATION ${PROJECT_SOURCE_DIR})