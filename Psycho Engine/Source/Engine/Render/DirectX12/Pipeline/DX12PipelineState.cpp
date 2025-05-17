#include "pch.h"
#include "DX12PipelineState.h"

namespace Psycho
{
	DX12PipelineState::~DX12PipelineState()
	{
		Release();
	}

	void DX12PipelineState::Initialize(ID3D12Device* device)
	{
		rootSignature.Initialize(device);
		shaders[0].Initialize(L"Shaders/VS.hlsl", HLSLShader::ShaderType::VERTEX);
		shaders[1].Initialize(L"Shaders/PS.hlsl", HLSLShader::ShaderType::PIXEL);

		D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDescription = {};
		pipelineDescription.pRootSignature = rootSignature.Get();
		pipelineDescription.VS.pShaderBytecode = shaders[0].GetByteBlob()->GetBufferPointer();
		pipelineDescription.VS.BytecodeLength = shaders[0].GetByteBlob()->GetBufferSize();
		pipelineDescription.PS.pShaderBytecode = shaders[1].GetByteBlob()->GetBufferPointer();
		pipelineDescription.PS.BytecodeLength = shaders[1].GetByteBlob()->GetBufferSize();
		
		pipelineDescription.BlendState.AlphaToCoverageEnable = false;
		pipelineDescription.BlendState.IndependentBlendEnable = false;
		pipelineDescription.BlendState.RenderTarget[0].BlendEnable = false;
		pipelineDescription.BlendState.RenderTarget[0].LogicOpEnable = false;
		pipelineDescription.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

		pipelineDescription.SampleMask = 0xFFFFFFFF;
		pipelineDescription.SampleDesc = { 1,0 };

		pipelineDescription.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
		pipelineDescription.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;
		pipelineDescription.RasterizerState.FrontCounterClockwise = false;
		pipelineDescription.RasterizerState.DepthClipEnable = true;
		pipelineDescription.RasterizerState.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

		pipelineDescription.DepthStencilState.DepthEnable = true;
		pipelineDescription.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
		pipelineDescription.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
		pipelineDescription.DepthStencilState.StencilEnable = false;
		pipelineDescription.DSVFormat = DXGI_FORMAT_D32_FLOAT;

		D3D12_INPUT_ELEMENT_DESC elements[] = {
			{ "POSITION" , 0 , DXGI_FORMAT_R32G32B32_FLOAT , 0 , 0 , D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA , 0 },
			{ "COLOR" , 0 , DXGI_FORMAT_R32G32B32A32_FLOAT , 0 , 12 , D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA , 0 } };


		D3D12_INPUT_LAYOUT_DESC inputLayout = {};
		inputLayout.NumElements = 2;
		inputLayout.pInputElementDescs = elements;

		pipelineDescription.InputLayout = inputLayout;

		pipelineDescription.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		pipelineDescription.NumRenderTargets = 1;;
		pipelineDescription.RTVFormats[0] = DXGI_FORMAT_R16G16B16A16_FLOAT;

		pipelineDescription.NodeMask = 0;

		pipelineDescription.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;

		EVALUATE_HR(device->CreateGraphicsPipelineState(&pipelineDescription, IID_PPV_ARGS(dx12PipelineState.GetAddressOf())), "Error Initializing The Graphics Pipeline State");
	}

	void DX12PipelineState::Release()
	{
		rootSignature.Release();
		shaders[0].Release();
		shaders[1].Release();

		if (dx12PipelineState)
		{
			dx12PipelineState.Reset();
		}
	}
}