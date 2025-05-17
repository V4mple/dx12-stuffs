#include "pch.h"
#include "DX12Debug.h"

namespace Psycho
{
	DX12Debug DX12Debug::instance;

	void DX12Debug::Enable()
	{
		GetInterface();
		dx12Debug->EnableDebugLayer();
	}

	void DX12Debug::GetInterface()
	{
		if (!dx12Debug)
		{
			EVALUATE_HR(D3D12GetDebugInterface(IID_PPV_ARGS(dx12Debug.GetAddressOf())), "Error Retrieving Debug Interface");
		}
	}
}