#include "pch.h"
#include "DXGISwapChain.h"

namespace Psycho
{
	DXGISwapChain::~DXGISwapChain()
	{
		Release();
	}

	void DXGISwapChain::Initialize(ID3D12Device* device, IDXGIFactory2* factory, ID3D12CommandQueue* commandQueue, const HWND hwnd, const UINT width, const UINT height)
	{
		this->width = width;
		this->height = height;

		D3D12_DESCRIPTOR_HEAP_DESC heapDescription = {};
		heapDescription.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		heapDescription.NodeMask = 0;
		heapDescription.NumDescriptors = MAX_SWAPCHAIN_BUFFERS;
		heapDescription.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;

		EVALUATE_HR(device->CreateDescriptorHeap(&heapDescription, IID_PPV_ARGS(bufferHeap.GetAddressOf())), "Error Initializing DXGI Buffer Heap");
		
		if (bufferHeap)
		{
			PRINT_N("Initialized DXGI Buffer Heap");
		}

		heapIncrement = device->GetDescriptorHandleIncrementSize(heapDescription.Type);

		DXGI_SWAP_CHAIN_DESC1 description = {};
		description.Height = height;
		description.Width = width;
		description.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
		description.Stereo = false;
		description.SampleDesc = { 1,0 };
		description.BufferUsage = { DXGI_USAGE_RENDER_TARGET_OUTPUT };
		description.BufferCount = MAX_SWAPCHAIN_BUFFERS;
		description.Scaling = DXGI_SCALING_NONE;
		description.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		description.AlphaMode = DXGI_ALPHA_MODE_IGNORE;
		description.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING | DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		EVALUATE_HR(factory->CreateSwapChainForHwnd(commandQueue, hwnd, &description, nullptr, nullptr, dxgiSwapChain.GetAddressOf()), "Error Initializing DXGI Swap Chain");

		if (dxgiSwapChain)
		{
			PRINT_N("Initialized DXGI Swap Chain");
		}

		bufferCount = MAX_SWAPCHAIN_BUFFERS;
		this->device = device;

		CreateBuffers();
	}

	void DXGISwapChain::CreateBuffers()
	{
		if (!dxgiSwapChain)
		{
			PRINT_N("Error: Unable To Locate Swap Chain");
		}

		for (int i = 0; i < bufferCount; i++)
		{
			buffers[i].Reset();
			EVALUATE_HR(dxgiSwapChain->GetBuffer(i, IID_PPV_ARGS(buffers[i].GetAddressOf())), "Error Initializing Buffer For Swap Chain. " << i);

			D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle = bufferHeap->GetCPUDescriptorHandleForHeapStart();
			cpuHandle.ptr += (size_t)heapIncrement * i;

			device->CreateRenderTargetView(buffers[i].Get(), 0, cpuHandle);
		}
	}

	void DXGISwapChain::DropBuffers()
	{
		for (int i = 0; i < bufferCount; i++)
		{
			buffers[i].Reset();
		}
	}

	void DXGISwapChain::Present()
	{
		dxgiSwapChain.Get()->Present(0, 0);
		currentBuffer = (currentBuffer + 1) % bufferCount;
	}

	D3D12_CPU_DESCRIPTOR_HANDLE DXGISwapChain::GetCurrentHandle()
	{
		D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle = bufferHeap->GetCPUDescriptorHandleForHeapStart();
		cpuHandle.ptr += (size_t)currentBuffer * heapIncrement;

		return cpuHandle;
	}

	void DXGISwapChain::Release()
	{
		if (dxgiSwapChain)
		{
			DropBuffers();
			bufferHeap.Reset();
			dxgiSwapChain.Reset();
		}
	}
}