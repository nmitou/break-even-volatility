cmake_minimum_required(VERSION 3.26.4)

project(BREAK-EVEN-VOLATILITY)

add_executable(${PROJECT_NAME} main.cpp bev.cpp utils.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC C:/local/eigen-3.4.0)