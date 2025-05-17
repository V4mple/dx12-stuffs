#pragma once

#include <wrl.h>
#include <d3d12.h>
#include <dxgi1_6.h>

namespace Psycho
{
	class DXGIAdapter
	{
	public:
		DXGIAdapter() = default;
		DXGIAdapter(Microsoft::WRL::ComPtr<IDXGIAdapter> adapter) : adapter(adapter) {}

		inline void Reset() { adapter.Reset(); }
		inline IDXGIAdapter* Get() const { return adapter.Get(); }

	private:
		Microsoft::WRL::ComPtr<IDXGIAdapter> adapter;
	};
}