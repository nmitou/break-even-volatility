cmake_minimum_required(VERSION 3.26.4)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(BREAK-EVEN-VOLATILITY)

add_executable(${PROJECT_NAME} main.cpp)

add_subdirectory(bev)

target_link_libraries(${PROJECT_NAME} PRIVATE BevClass)

target_include_directories(${PROJECT_NAME} 
						   PRIVATE "C:/local/eigen-3.4.0"
						   PRIVATE "${PROJECT_SOURCE_DIR}/bev")