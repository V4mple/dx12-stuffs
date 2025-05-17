#include "pch.h"
#include "DX12CommandQueue.h"

namespace Psycho
{
	DX12CommandQueue::~DX12CommandQueue()
	{
		ReleaseQueue();
	}

	void DX12CommandQueue::Initialize(ID3D12Device* device)
	{
		D3D12_COMMAND_QUEUE_DESC description = {};

		description.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
		description.Priority = D3D12_COMMAND_QUEUE_PRIORITY_HIGH;
		description.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		description.NodeMask = 0;

		EVALUATE_HR(device->CreateCommandQueue( &description, IID_PPV_ARGS(commandQueue.GetAddressOf())), "Error Creating DX12 Command Queue");

		if (commandQueue)
		{
			PRINT_N("Initialized DX12 Command Queue");
		}

		EVALUATE_HR(device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(fence.GetAddressOf())), "Error Creating DX12 Fence");

		if (fence)
		{
			PRINT_N("Initialized DX12 Fence");
		}
	}

	void DX12CommandQueue::ExecuteCommandList(ID3D12CommandList* pCommandList)
	{
		Get()->ExecuteCommandLists(1, (ID3D12CommandList* const*)&pCommandList);
		currentFenceValue++;
		Get()->Signal(fence.Get(), currentFenceValue);
	}
	void DX12CommandQueue::ReleaseQueue()
	{
		if (commandQueue.Get())
		{
			commandQueue.Reset();
		}

		if (fence.Get())
		{
			fence.Reset();
		}
	}
	void DX12CommandQueue::FlushQueue()
	{
		if (Get())
		{
			for (unsigned int i = 0; i < MAX_SWAPCHAIN_BUFFERS; i++)
			{
				Get()->Signal(fence.Get(), currentFenceValue + i);
			}
		}
	}
}