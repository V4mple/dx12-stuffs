#pragma once

#include <wrl.h>
#include <d3d12.h>
#include <dxgi1_6.h>

namespace Psycho
{
	class GraphicsAPI DX12Device
	{
	public:
		DX12Device() = default;
		DX12Device(IDXGIAdapter* adapter);

		void Initialize(IDXGIAdapter* adapter);

		inline void Reset() { device.Reset(); }
		inline void SetName(const wchar_t* name) { if (device) device->SetName(name); }
		inline ID3D12Device* Get() const { return device.Get(); }

	private:
		Microsoft::WRL::ComPtr<ID3D12Device> device;
	};
}