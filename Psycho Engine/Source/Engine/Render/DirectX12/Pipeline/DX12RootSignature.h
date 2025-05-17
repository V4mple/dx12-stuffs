#pragma once
#include <wrl.h>

namespace Psycho
{
	class DX12RootSignature
	{
	public:
		DX12RootSignature() = default;
		~DX12RootSignature();

		void Initialize(ID3D12Device* device);
		void Release();

		inline ID3D12RootSignature* Get() const { return dx12RootSignature.Get(); }
		
	private:
		Microsoft::WRL::ComPtr<ID3D12RootSignature> dx12RootSignature;
	};
}