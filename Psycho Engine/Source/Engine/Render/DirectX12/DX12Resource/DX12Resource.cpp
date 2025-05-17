#include "pch.h"
#include "DX12Resource.h"

namespace Psycho
{
	DX12Resource::~DX12Resource()
	{
		Release();
	}
	void DX12Resource::Initialize(ID3D12Device* device, const unsigned int numBytes, D3D12_HEAP_TYPE heapType, D3D12_RESOURCE_STATES initialState)
	{
		D3D12_HEAP_PROPERTIES heapProperties = {};
		heapProperties.Type = heapType;
		heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
		heapProperties.CreationNodeMask = 0;
		heapProperties.VisibleNodeMask = 0;

		D3D12_RESOURCE_DESC resourceDescription = {};
		resourceDescription.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		resourceDescription.Alignment = 0;
		resourceDescription.Width = numBytes;
		resourceDescription.Height = 1;
		resourceDescription.DepthOrArraySize = 1;
		resourceDescription.MipLevels = 1;
		resourceDescription.Format = DXGI_FORMAT_UNKNOWN;
		resourceDescription.SampleDesc = { 1,0 };
		resourceDescription.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		resourceDescription.Flags = D3D12_RESOURCE_FLAG_NONE;

		EVALUATE_HR(device->CreateCommittedResource(&heapProperties, D3D12_HEAP_FLAG_NONE, &resourceDescription, initialState, 0, IID_PPV_ARGS(dxResource.GetAddressOf())), "Error Initializing DX12 Resource");
	}
	void DX12Resource::Release()
	{
		if (memory && Get())
		{
			Get()->Unmap(0, 0);
		}

		if (dxResource)
		{
			dxResource.Reset();
		}
	}
	void* DX12Resource::GetCPUMemory()
	{
		if (!memory)
		{
			Get()->Map(0, 0, &memory);
		}
		return memory;
	}
}