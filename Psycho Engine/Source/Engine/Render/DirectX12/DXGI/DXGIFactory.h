#pragma once

#include <wrl.h>

#include "DXGIAdapter.h"

namespace Psycho
{
	class DXGIFactory
	{
	public:
		DXGIFactory();
		DXGIAdapter GetAdapter();

		inline IDXGIFactory2* Get() const { return factory.Get(); }

	private:
		Microsoft::WRL::ComPtr<IDXGIFactory2> factory;
	};
}