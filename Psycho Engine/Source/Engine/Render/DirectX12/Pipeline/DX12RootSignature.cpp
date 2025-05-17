#include "pch.h"
#include "DX12RootSignature.h"

namespace Psycho
{
	DX12RootSignature::~DX12RootSignature()
	{
		Release();
	}

	void DX12RootSignature::Initialize(ID3D12Device* device)
	{
		D3D12_ROOT_PARAMETER rootParameter[1];
		rootParameter[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
		rootParameter[0].Descriptor.ShaderRegister = 0;
		rootParameter[0].Descriptor.RegisterSpace = 0;
		rootParameter[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;

		D3D12_VERSIONED_ROOT_SIGNATURE_DESC rootDescription = {};
		rootDescription.Version = D3D_ROOT_SIGNATURE_VERSION_1_0;
		rootDescription.Desc_1_0.pParameters = rootParameter;
		rootDescription.Desc_1_0.NumParameters = 1;
		rootDescription.Desc_1_0.NumStaticSamplers = 0;
		rootDescription.Desc_1_0.pStaticSamplers = 0;
		rootDescription.Desc_1_0.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

		ID3DBlob* serializedRootSignature = nullptr;
		ID3DBlob* errors = nullptr;

		EVALUATE_HR(D3D12SerializeVersionedRootSignature(&rootDescription, &serializedRootSignature, &errors), "Error Serializing The Root Signature");

		if (errors)
		{
			PRINT_N("Root Signature Serialization Error: " << (const char*)errors->GetBufferPointer());
		}

		EVALUATE_HR(device->CreateRootSignature(0, serializedRootSignature->GetBufferPointer(), serializedRootSignature->GetBufferSize(), IID_PPV_ARGS(dx12RootSignature.GetAddressOf())), "Error Creating The Root Signature");
	}

	void DX12RootSignature::Release()
	{
		if (dx12RootSignature)
		{
			dx12RootSignature.Reset();
		}
	}
}