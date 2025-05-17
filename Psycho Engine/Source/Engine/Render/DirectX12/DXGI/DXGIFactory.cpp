#include "pch.h"
#include "DXGIFactory.h"

namespace Psycho
{
	using namespace Microsoft::WRL;

	DXGIFactory::DXGIFactory()
	{
		EVALUATE_HR(CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG, IID_PPV_ARGS(factory.GetAddressOf())),"Unknown Error");
	}
	DXGIAdapter DXGIFactory::GetAdapter()
	{
		ComPtr<IDXGIFactory6> factorySix;
		ComPtr<IDXGIAdapter> adapter;

		if (factory.Get()->QueryInterface(IID_PPV_ARGS(&factorySix)) == S_OK)
		{
			EVALUATE_HR(factorySix->EnumAdapterByGpuPreference(0, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&adapter)), "Error Finding Adapter");
		}
		else
		{
			EVAL_ASSERT(false);
		}

		return DXGIAdapter(adapter);
	}
}