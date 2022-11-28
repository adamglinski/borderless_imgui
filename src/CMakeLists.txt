set(OUTPUT_BIN_NAME imgui_external)
include(CMakePrintHelpers)

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS")

set(DX9_LIB_DIR "C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Lib/x86")
set(DX9_INCLUDE_DIR "C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Include")

cmake_print_variables(DX9_LIB_DIR)

add_subdirectory(gui)

add_executable(${OUTPUT_BIN_NAME} main.cpp )
# target_include_directories(${OUTPUT_BIN_NAME} PUBLIC ../${IMGUI_DIR})
target_include_directories(${OUTPUT_BIN_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${OUTPUT_BIN_NAME} PUBLIC imgui gui)

target_link_libraries(${OUTPUT_BIN_NAME} PUBLIC ${DX9_LIB_DIR}/d3d9.lib)
target_include_directories(${OUTPUT_BIN_NAME} PUBLIC ${DX9_INCLUDE_DIR})

# link_directories("C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Lib/x86")
# include_directories("C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Include")