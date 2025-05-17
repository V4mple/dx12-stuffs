#pragma once

#include <wrl.h>
#include <d3d12.h>
#include <dxgi1_6.h>

namespace Psycho
{
	class DX12Debug
	{
	public:
		DX12Debug() = default;

		static inline DX12Debug& Get() { return instance; }
		void Enable();

	private:
		Microsoft::WRL::ComPtr<ID3D12Debug> dx12Debug;
		static DX12Debug instance;
		void GetInterface();
	};
}