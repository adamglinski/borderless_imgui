#include "gui/gui.h"

#include <thread>

int __stdcall wWinMain(HINSTANCE instance, HINSTANCE prev_instance, PWSTR args, int cmd_show) {
	gui::create_window(L"imgui_external");
	gui::create_device();
	gui::create_imgui();

	while(gui::is_running){
		gui::begin_render();
		gui::render();
		gui::end_render();
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}

	gui::destroy_imgui();
	gui::destroy_device();
	gui::destroy_window();

	return EXIT_SUCCESS;
}