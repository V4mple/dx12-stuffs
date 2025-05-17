#include "pch.h"
#include "Application.h"

#include "Engine/Render/DirectX12/Debug/DXGIDebug.h"

namespace Psycho
{
	LRESULT CALLBACK WindProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {

		switch (msg)
		{
			case WM_NCCREATE:
			{
				LPCREATESTRUCT param = reinterpret_cast<LPCREATESTRUCT>(lparam);
				Application* pointer = reinterpret_cast<Application*>(param->lpCreateParams);
				SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pointer));
				std::cout << "Sent create message" << std::endl;
				break;
			}
			case WM_CREATE:
			{
				Application* pointer = reinterpret_cast<Application*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
				pointer->OnCreate(hwnd);
				break;
			}
			case WM_DESTROY:
			{
				Application* pointer = reinterpret_cast<Application*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
				pointer->OnDestroy();
				PostQuitMessage(0);
				break;
			}
			case WM_ERASEBKGND:
			{
				HDC hdc = reinterpret_cast<HDC>(wparam);
				RECT rect;
				GetClientRect(hwnd, &rect);
				HBRUSH darkBrush = CreateSolidBrush(RGB(30, 30, 30));
				FillRect(hdc, &rect, darkBrush);
				DeleteObject(darkBrush);
				return 1;
			}
			case WM_CTLCOLORSTATIC:
			case WM_CTLCOLORBTN:
			case WM_CTLCOLORDLG:
			case WM_CTLCOLOREDIT:
			{
				HDC hdcStatic = (HDC)wparam;
				SetTextColor(hdcStatic, RGB(255, 255, 255));
				SetBkColor(hdcStatic, RGB(30, 30, 30));
				return (LRESULT)CreateSolidBrush(RGB(30, 30, 30));
			}
		}
		return DefWindowProc(hwnd, msg, wparam, lparam);
	}

	bool Application::Initialize()
	{
		WNDCLASS wndClass = {};
		wndClass.lpszClassName = L"BaseWindowClass";
		wndClass.style = 0;
		wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
		wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		wndClass.hbrBackground = (HBRUSH)COLOR_WINDOW;
		wndClass.lpszMenuName = 0;
		wndClass.hInstance = 0;
		wndClass.lpfnWndProc = WindProc;
		wndClass.cbClsExtra = 0;
		wndClass.cbWndExtra = 0;

		RegisterClass(&wndClass);

		windowHandle = CreateWindow(L"BaseWindowClass", L"Window Name", WS_OVERLAPPEDWINDOW, 200, 200, windowWidth, windowHeight, 0, 0, 0, this);

		BOOL useDarkMode = TRUE;
		DwmSetWindowAttribute(windowHandle, DWMWA_USE_IMMERSIVE_DARK_MODE, &useDarkMode, sizeof(useDarkMode));

		if (!windowHandle)
		{
			return false;
		}

		ShowWindow(windowHandle, SW_SHOW);

		UpdateWindow(windowHandle);

		windowStatus = true;

		return windowStatus;
	}

	void Application::OnCreate(HWND hwnd)
	{
		std::cout << "Window Created" << std::endl;

		renderer.Initialize(hwnd,windowWidth,windowHeight);

	}

	void Application::Update()
	{
		MSG message;

		while (PeekMessage(&message, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
		if (windowStatus) {
			// Calculate delta time
			static float lastTime = static_cast<float>(GetTickCount64()) / 1000.0f;
			float currentTime = static_cast<float>(GetTickCount64()) / 1000.0f;
			float dt = currentTime - lastTime;
			lastTime = currentTime;

			// Camera movement
			float moveSpeed = renderer.GetCameraSpeed() * dt;
			DirectX::XMFLOAT3& camPos = renderer.GetCameraPosition();
			if (GetAsyncKeyState('W') & 0x8000) camPos.z += moveSpeed; // Forward
			if (GetAsyncKeyState('S') & 0x8000) camPos.z -= moveSpeed; // Backward
			if (GetAsyncKeyState('A') & 0x8000) camPos.x -= moveSpeed; // Left
			if (GetAsyncKeyState('D') & 0x8000) camPos.x += moveSpeed; // Right
			if (GetAsyncKeyState('Q') & 0x8000) camPos.y -= moveSpeed; // Down
			if (GetAsyncKeyState('E') & 0x8000) camPos.y += moveSpeed; // Up

			renderer.UpdateDraw(dt); // Pass delta time
		}
	}

	void Application::OnDestroy()
	{
		windowStatus = false;

		std::cout << "Window Closed" << std::endl;

		renderer.Release();

		DXGIDebug::Get().GetLiveObjects();
	}

	bool Application::IsRunning() const
	{
		return windowStatus;
	}
}