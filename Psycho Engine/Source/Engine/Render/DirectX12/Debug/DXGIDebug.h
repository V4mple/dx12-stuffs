#pragma once

#include <wrl.h>
#include <d3d12.h>
#include <dxgi1_6.h>

namespace Psycho
{
	class DXGIDebug
	{
	public:
		DXGIDebug() = default;

		void Enable();
		void GetLiveObjects();

		static inline DXGIDebug& Get() { return instance; }

	private:
		Microsoft::WRL::ComPtr<IDXGIDebug> dxgiDebug;
		void GetInterface();
		static DXGIDebug instance;
	};
}