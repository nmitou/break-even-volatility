add_library(BevClass bev.cpp)

add_subdirectory(utils)

target_link_libraries(BevClass PRIVATE Utils)

target_include_directories(BevClass 
							PUBLIC "C:/local/eigen-3.4.0"
							PRIVATE "./utils")