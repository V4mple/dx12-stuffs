#include "pch.h"
#include "DX12CommandList.h"

namespace Psycho {
	DX12CommandList::~DX12CommandList()
	{
		Release();
	}

	void DX12CommandList::Initialize(ID3D12Device* device)
	{
		EVALUATE_HR(device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(allocator.GetAddressOf())), "Error creating command allocator");
		if (allocator)
		{
			PRINT_N("Initialized DX12 Command Allocator");
		}

		EVALUATE_HR(device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, allocator.Get(), nullptr, IID_PPV_ARGS(commandList.GetAddressOf())), "Error creating the commandlist");
		if (commandList)
		{
			PRINT_N("Initialized DX12 Command List");
		}
	}

	void DX12CommandList::ResetCommandList()
	{
		allocator.Get()->Reset();

		gpuCommand()->Reset(allocator.Get(), 0);
	}

	void DX12CommandList::Release()
	{
		if (commandList.Get())
		{
			commandList.Reset();
		}

		if (allocator.Get())
		{
			allocator.Reset();
		}
	}
}