set(OUTPUT_BIN_NAME imgui_external)

set(IMGUI_DIR ext/imgui)

add_executable(${OUTPUT_BIN_NAME} main.cpp)
target_include_directories(${OUTPUT_BIN_NAME} PUBLIC ${IMGUI_DIR})
target_include_directories(${OUTPUT_BIN_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

