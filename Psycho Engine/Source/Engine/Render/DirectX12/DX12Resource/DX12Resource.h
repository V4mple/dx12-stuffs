#pragma once
#include <wrl.h>

namespace Psycho
{
	class DX12Resource
	{
	public:
		DX12Resource() = default;
		~DX12Resource();

		void Initialize(ID3D12Device* device, const unsigned int numBytes, D3D12_HEAP_TYPE heapType, D3D12_RESOURCE_STATES initialState);
		void Release();

		inline ID3D12Resource* const Get() { return dxResource.Get(); }
		void* GetCPUMemory();

	private:
		Microsoft::WRL::ComPtr<ID3D12Resource> dxResource;
		void* memory = nullptr;
	};
}