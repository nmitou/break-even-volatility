add_library(BevClass bev.cpp)

add_subdirectory(utils)

target_link_libraries(BevClass PRIVATE Utils)

target_include_directories(BevClass 
							PUBLIC ${PATH_TO_EIGEN_LIB}
							PRIVATE "./utils")