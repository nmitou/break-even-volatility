set(EX1 example1)

add_executable(${EX1} example1_GBMdata.cpp)

set_target_properties(${EX1} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${EX1}
						PRIVATE BevClass
						PRIVATE Utils)

target_include_directories(${EX1} 
							PUBLIC ${PATH_TO_EIGEN_LIB}
							PRIVATE "${PROJECT_SOURCE_DIR}/bev"
							PRIVATE "${PROJECT_SOURCE_DIR}/bev/utils")