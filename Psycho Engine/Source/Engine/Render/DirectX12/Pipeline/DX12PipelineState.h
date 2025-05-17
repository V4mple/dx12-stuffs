#pragma once
#include <wrl.h>
#include "DX12RootSignature.h"
#include "HLSLShader.h"

namespace Psycho
{
	class DX12PipelineState
	{
	public:
		DX12PipelineState() = default;
		~DX12PipelineState();

		void Initialize(ID3D12Device* device);
		void Release();

		inline ID3D12PipelineState* const Get() { return dx12PipelineState.Get(); }
		inline ID3D12RootSignature* const GetRootSignature() { return rootSignature.Get(); }

	private:
		Microsoft::WRL::ComPtr<ID3D12PipelineState> dx12PipelineState;
		DX12RootSignature rootSignature;
		HLSLShader shaders[2];
	};
}