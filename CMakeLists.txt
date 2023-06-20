cmake_minimum_required(VERSION 3.26.4)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(PATH_TO_EIGEN_LIB "C:/local/eigen-3.4.0")

project(BREAK-EVEN-VOLATILITY)

add_executable(main main.cpp)

set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

add_subdirectory(bev)
add_subdirectory(examples)

target_link_libraries(${PROJECT_NAME}
						PRIVATE BevClass
						PRIVATE Utils)

target_include_directories(${PROJECT_NAME} 
							PRIVATE ${PATH_TO_EIGEN_LIB}
							PRIVATE "${PROJECT_SOURCE_DIR}/bev"
							PRIVATE "${PROJECT_SOURCE_DIR}/bev/utils")