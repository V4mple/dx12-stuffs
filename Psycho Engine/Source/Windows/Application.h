#pragma once

#ifndef WIN32_LEAN_AND_MEAN
	#define WIN32_LEAN_AND_MEAN
#endif

#ifndef NOMINMAX
	#define NOMINMAX
#endif

#include "Minimal.h"

#include <Windows.h>

#include <dwmapi.h>
#pragma comment(lib, "dwmapi.lib")

#include "Engine/Render/Renderer.h"

namespace Psycho
{
	class GraphicsAPI Application
	{
	public:
		Application() = default;

		bool Initialize();

		void OnCreate(HWND hwnd);
		void Update();
		void OnDestroy();

		bool IsRunning() const;

	private:
		bool windowStatus = false;

		UINT windowWidth = 1440;
		UINT windowHeight = 1080;

		HWND windowHandle = nullptr;

		Renderer renderer;
	};
}