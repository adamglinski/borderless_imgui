cmake_minimum_required(VERSION 3.23)

set(CMAKE_GENERATOR_PLATFORM win32)
set(CMAKE_CXX_STANDARD 20)
set(IMGUI_DIR ./ext/imgui)


project(imgui_external CXX)

# imgui #
add_library(imgui STATIC 
	${IMGUI_DIR}/imgui.cpp 
 	${IMGUI_DIR}/imgui.h 
	${IMGUI_DIR}/imstb_rectpack.h 
 	${IMGUI_DIR}/imstb_textedit.h 
	${IMGUI_DIR}/imstb_truetype.h 
	${IMGUI_DIR}/imgui_demo.cpp 
	${IMGUI_DIR}/imgui_draw.cpp 
	${IMGUI_DIR}/imgui_internal.h 
	${IMGUI_DIR}/imgui_tables.cpp 
	${IMGUI_DIR}/imgui_widgets.cpp 
	${IMGUI_DIR}/imgui_impl_dx9.cpp 
	${IMGUI_DIR}/imgui_impl_dx9.h  
	${IMGUI_DIR}/imgui_impl_win32.cpp 
	${IMGUI_DIR}/imgui_impl_win32.h 
	)

# target_include_directories(imgui PUBLIC $<BUILD_INTERFACE:${IMGUI_DIR}> $<INSTALL_INTERFACE:include>)

install(TARGETS imgui DESTINATION lib)

add_subdirectory(src)