#include "gui.h"

/* imgui */
#include "../../ext/imgui/imgui.h"
#include "../../ext/imgui/imgui_impl_dx9.h"
#include "../../ext/imgui/imgui_impl_win32.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND window, UINT msg, WPARAM wParam, LPARAM lParam);

long __stdcall window_process(HWND window, UINT msg, WPARAM wParam, LPARAM lParam) {
	if (ImGui_ImplWin32_WndProcHandler(window, msg, wParam, lParam))
		return true;

	switch (msg){
	case WM_SIZE: {
		if (gui::device && wParam != SIZE_MINIMIZED) {
			gui::present_params.BackBufferWidth = LOWORD(lParam);
			gui::present_params.BackBufferHeight = HIWORD(lParam);
			gui::reset_device();
		}
	}return 0;

	case WM_SYSCOMMAND: {
		if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT menu
			return 0;
	}break;

	case WM_DESTROY: {
		PostQuitMessage(0);
	} return 0;

	case WM_LBUTTONDOWN: {
		gui::position = MAKEPOINTS(lParam);
	} return 0;

	case WM_MOUSEMOVE: {
		if (wParam == MK_LBUTTON) {
			const auto points = MAKEPOINTS(lParam);
			auto rect = ::RECT{};
			GetWindowRect(gui::window, &rect);

			rect.left += points.x - gui::position.x;
			rect.top += points.y - gui::position.y;

			if (gui::position.x >= 0 && gui::position.x <= gui::width_c && gui::position.y >= 0 && gui::position.y <= 19)
				SetWindowPos(gui::window, HWND_TOPMOST, rect.left, rect.top, 0, 0, SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOZORDER);
		}
	} return 0;
	}

	return DefWindowProcW(window, msg, wParam, lParam);
}

void gui::create_window(const wchar_t* window_name) noexcept{
	gui::window_class.cbSize = sizeof(WNDCLASSEX);
	gui::window_class.style = CS_CLASSDC;
	gui::window_class.lpfnWndProc = window_process;
	gui::window_class.cbClsExtra = 0;
	gui::window_class.cbWndExtra = 0;
	gui::window_class.hInstance = GetModuleHandleA(0);
	gui::window_class.hIcon = 0;
	gui::window_class.hCursor = 0;
	gui::window_class.hbrBackground = 0;
	gui::window_class.lpszMenuName = 0;
	gui::window_class.lpszClassName = L"class001";
	gui::window_class.hIconSm = 0;

	RegisterClassEx(&gui::window_class);

	window = CreateWindowEx(
		0,
		L"class001",
		window_name,
		WS_POPUP,
		100,
		100,
		width_c,
		height_c,
		0,
		0,
		gui::window_class.hInstance,
		0
	);

	ShowWindow(window, SW_SHOWDEFAULT);
	UpdateWindow(window);
}

void gui::destroy_window() noexcept{
	DestroyWindow(gui::window);
	UnregisterClass(window_class.lpszClassName, window_class.hInstance);
}

bool gui::create_device() noexcept{
	d3d = Direct3DCreate9(D3D_SDK_VERSION);

	if (!d3d)
		return false;

	ZeroMemory(&present_params, sizeof(present_params));

	present_params.Windowed = TRUE;
	present_params.SwapEffect = D3DSWAPEFFECT_DISCARD;
	present_params.BackBufferFormat = D3DFMT_UNKNOWN;
	present_params.EnableAutoDepthStencil = TRUE;
	present_params.AutoDepthStencilFormat = D3DFMT_D16;
	present_params.PresentationInterval = D3DPRESENT_INTERVAL_ONE;

	if (d3d->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		window,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&present_params,
		&device) < 0)
		return false;

	return true;
}

void gui::reset_device() noexcept{
	ImGui_ImplDX9_InvalidateDeviceObjects();

	const auto result = device->Reset(&present_params);

	if (result == D3DERR_INVALIDCALL)
		IM_ASSERT(0);

	ImGui_ImplDX9_CreateDeviceObjects();
}

void gui::destroy_device() noexcept{
	if (device)
	{
		device->Release();
		device = nullptr;
	}

	if (d3d)
	{
		d3d->Release();
		d3d = nullptr;
	}
}

void gui::create_imgui() noexcept{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ::ImGui::GetIO();

	io.IniFilename = NULL;

	ImGui::StyleColorsDark();

	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX9_Init(device);
}

void gui::destroy_imgui() noexcept{
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void gui::begin_render() noexcept{
	MSG message;
	while (PeekMessage(&message, 0, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&message);
		DispatchMessage(&message);

		if (message.message == WM_QUIT)
		{
			is_running = !is_running;
			return;
		}
	}

	// Start the Dear ImGui frame
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void gui::end_render() noexcept{

	ImGui::EndFrame();

	device->SetRenderState(D3DRS_ZENABLE, FALSE);
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	device->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);

	device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(0, 0, 0, 255), 1.0f, 0);

	if (device->BeginScene() >= 0)
	{
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
		device->EndScene();
	}

	const auto result = device->Present(0, 0, 0, 0);

	// Handle loss of D3D9 device
	if (result == D3DERR_DEVICELOST && device->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
		reset_device();
}

void gui::render() noexcept{
	ImGui::SetNextWindowPos({ 0, 0 });
	ImGui::SetNextWindowSize({ width_c, height_c });
	ImGui::Begin(
		"main window",
		&is_running,
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoSavedSettings |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoMove
	);

	ImGui::Button("wsp");

	ImGui::End();
}
