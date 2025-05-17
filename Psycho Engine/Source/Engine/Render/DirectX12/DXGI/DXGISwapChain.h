#pragma once

#include <wrl.h>

#include "EngineSettings.h"

namespace Psycho
{
	class GraphicsAPI DXGISwapChain
	{
	public:
		DXGISwapChain() = default;
		~DXGISwapChain();

		void Initialize(ID3D12Device* device, IDXGIFactory2* factory, ID3D12CommandQueue* commandQueue, const HWND hwnd, const UINT width, const UINT height);
		void CreateBuffers();
		void DropBuffers();
		void Present();
		void Release();

		D3D12_CPU_DESCRIPTOR_HANDLE GetCurrentHandle();
		inline ID3D12Resource* GetCurrentRenderTarget() { return buffers[currentBuffer].Get(); }

	private:
		ID3D12Device* device = nullptr;
		Microsoft::WRL::ComPtr<IDXGISwapChain1> dxgiSwapChain;
		Microsoft::WRL::ComPtr<ID3D12Resource> buffers[MAX_SWAPCHAIN_BUFFERS];
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> bufferHeap;


		UINT bufferCount = 0;
		UINT currentBuffer = 0;

		UINT width = 0;
		UINT height = 0;

		UINT heapIncrement = 0;

	};
};