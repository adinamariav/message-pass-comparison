cmake_minimum_required(VERSION 3.13)
project(message-pass-comparison)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory("${PROJECT_SOURCE_DIR}/client-server" "${PROJECT_SOURCE_DIR}/client-server/client-server-output")
add_subdirectory("${PROJECT_SOURCE_DIR}/peer2peer" "${PROJECT_SOURCE_DIR}/peer2peer/peer2peer-output")