cmake_minimum_required(VERSION 3.26.4)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(PATH_TO_EIGEN_LIB "C:/local/eigen-3.4.0")

project(BREAK-EVEN-VOLATILITY)

add_executable(${PROJECT_NAME} main.cpp)

add_subdirectory(bev)

target_link_libraries(${PROJECT_NAME} PRIVATE BevClass)

target_include_directories(${PROJECT_NAME} 
							PRIVATE ${PATH_TO_EIGEN_LIB}
							PRIVATE "${PROJECT_SOURCE_DIR}/bev")