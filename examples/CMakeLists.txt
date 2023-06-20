set(EX1 example1)
set(EX_PNLS example_pnls)

set(EX_PROPS	RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
set(EX_LINK_LIBS 	PRIVATE BevClass 
					PRIVATE Utils)
set(EX_INCLUDE_DIRS		PUBLIC ${PATH_TO_EIGEN_LIB} 
						PRIVATE "${PROJECT_SOURCE_DIR}/bev" 
						PRIVATE "${PROJECT_SOURCE_DIR}/bev/utils")

add_executable(${EX1} example1_GBMdata.cpp)
add_executable(${EX_PNLS} example_investigating_pnls.cpp)

set_target_properties(${EX1} PROPERTIES ${EX_PROPS})
set_target_properties(${EX_PNLS} PROPERTIES ${EX_PROPS})

target_link_libraries(${EX1} ${EX_LINK_LIBS})
target_link_libraries(${EX_PNLS} ${EX_LINK_LIBS})

target_include_directories(${EX1} ${EX_INCLUDE_DIRS})
target_include_directories(${EX_PNLS} ${EX_INCLUDE_DIRS})