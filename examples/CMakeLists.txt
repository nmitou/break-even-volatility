add_executable(ex1 example1_GBMdata.cpp)

set_target_properties(ex1 PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(ex1 PRIVATE BevClass)

target_include_directories(ex1 
							PUBLIC ${PATH_TO_EIGEN_LIB}
							PRIVATE "../bev")