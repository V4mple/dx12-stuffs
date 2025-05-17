#pragma once

#include <wrl.h>
#include <d3d12.h>
#include <dxgi1_6.h>

#include "EngineSettings.h"

namespace Psycho
{
	class GraphicsAPI DX12CommandQueue
	{
	public:
		DX12CommandQueue() = default;
		~DX12CommandQueue();


		void Initialize(ID3D12Device* device);
		void ExecuteCommandList(ID3D12CommandList* pCommandList);
		void ReleaseQueue();
		void FlushQueue();

		inline ID3D12CommandQueue* Get() const { return commandQueue.Get(); }
		inline ID3D12Fence* GetFence() { return fence.Get(); }
		inline UINT64 GetCurrentFenceValue() const { return currentFenceValue; }

	private:
		Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue;
		Microsoft::WRL::ComPtr<ID3D12Fence> fence;
		UINT64 currentFenceValue = 0;
	};
}