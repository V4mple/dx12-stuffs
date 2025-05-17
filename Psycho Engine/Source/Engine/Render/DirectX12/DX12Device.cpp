#include "pch.h"
#include "DX12Device.h"

namespace Psycho
{
	DX12Device::DX12Device(IDXGIAdapter* adapter)
	{
		Initialize(adapter);
	}

	void DX12Device::Initialize(IDXGIAdapter* adapter)
	{
		EVALUATE_HR(D3D12CreateDevice(adapter, D3D_FEATURE_LEVEL_12_1, IID_PPV_ARGS(device.GetAddressOf())), "Error Initializing Device.");

		if (device)
		{
			PRINT_N("Initialized DX12 Device");
		}
	}
}