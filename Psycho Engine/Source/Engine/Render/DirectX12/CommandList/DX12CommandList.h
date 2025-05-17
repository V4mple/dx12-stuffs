#pragma once

#include <wrl.h>
#include <d3d12.h>
#include <dxgi1_6.h>

namespace Psycho
{
	class GraphicsAPI DX12CommandList
	{
	public:
		DX12CommandList() = default;
		~DX12CommandList();

		void Initialize(ID3D12Device* device);
		void ResetCommandList();
		void Release();

		inline ID3D12CommandList* Get() const { return commandList.Get(); }
		inline ID3D12GraphicsCommandList* gpuCommand() { return commandList.Get(); }

	private:
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList;
		Microsoft::WRL::ComPtr<ID3D12CommandAllocator> allocator;
	};
}