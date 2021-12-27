cmake_minimum_required(VERSION 3.21)
project(gacha64server)

set(CMAKE_CXX_STANDARD 20)

include_directories("/usr/local/include")
link_directories("/usr/local/lib")

add_executable(${CMAKE_PROJECT_NAME} src/main.cpp)

target_link_libraries(${CMAKE_PROJECT_NAME} PUBLIC enet)