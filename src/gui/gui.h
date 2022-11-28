#pragma once
#ifndef GUI_H
#define GUI_H

#include <d3d9.h>

namespace gui{
	constexpr int width_c = 500;
	constexpr int height_c = 300;

	inline bool is_running = true;

	/* winapi */
	inline HWND window = nullptr;
	inline WNDCLASSEX window_class = {};

	inline POINTS position = {};

	/* direct x */
	inline PDIRECT3D9 d3d = nullptr;
	inline LPDIRECT3DDEVICE9 device = nullptr;
	inline D3DPRESENT_PARAMETERS present_params = {};

	void create_window(const wchar_t* window_name) noexcept;
	void destroy_window() noexcept;

	bool create_device() noexcept;
	void reset_device() noexcept;
	void destroy_device() noexcept;

	void create_imgui() noexcept;
	void destroy_imgui() noexcept;

	void begin_render() noexcept;
	void end_render() noexcept;
	void render() noexcept;

}

#endif /* GUI_H */