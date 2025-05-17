#pragma once

#include "Minimal.h"
#include "RenderDataTypes.h"

#include <Windows.h>
#include <vector>

#include "DirectX12/DXGI/DXGIFactory.h"
#include "DirectX12/DXGI/DXGIAdapter.h"
#include "DirectX12/Debug/DX12Debug.h"

#include "DirectX12/DX12Device.h"
#include "DirectX12/CommandQueue/DX12CommandQueue.h"
#include "DirectX12/CommandList/DX12CommandList.h"
#include "DirectX12/DXGI/DXGISwapChain.h"
#include "DirectX12/DX12Resource/DX12Resource.h"
#include "DirectX12/Pipeline/HLSLShader.h"
#include "DirectX12/Pipeline/DX12RootSignature.h"
#include "DirectX12/Pipeline/DX12PipelineState.h"

#include "Object Loader/ObjectLoader.h"
#include "../Utilities/Utilities.h"

namespace Psycho
{
	class GraphicsAPI Renderer
	{
	public:
		Renderer() = default;
		~Renderer();

		float GetCameraSpeed() const { return cameraSpeed; }
		DirectX::XMFLOAT3& GetCameraPosition() { return cameraPosition; }

		void Release();
		void UpdateDraw(float dt);
		void Initialize(HWND hwnd, UINT width = 0, UINT height = 0);

	private:
		DX12Device device;
		DX12CommandQueue commandQueue;
		DX12CommandList commandList;
		DXGISwapChain swapChain;
		DX12PipelineState pipeline;
		DX12Resource dynamicVertexBuffer;
		DX12Resource dynamicIndexBuffer;
		D3D12_VERTEX_BUFFER_VIEW dynamicVertexBufferView;
		D3D12_INDEX_BUFFER_VIEW dynamicIndexBufferView;

		Microsoft::WRL::ComPtr<ID3D12Resource> depthBuffer;
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> depthHeap;

		DX12Resource constantBufferPassData;

		D3D12_VIEWPORT viewPort;
		D3D12_RECT dx12Rect;

		DirectX::XMMATRIX viewProjectionMatrix;

		ObjectLoader objectLoader;

		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;
		UINT indexCount;

		DirectX::XMFLOAT3 cameraPosition = { 0.0f, 1.0f, -5.0f };
		DirectX::XMFLOAT3 cameraTargetOffset = { 0.0f, 0.0f, 5.0f }; // Look 5 units ahead

		float pitchAngle = 0.0f;
		float cameraSpeed = 100.0f;
		float cubeRotationAngle = 0.0f; // Separate rotation for cube
		float lastFrameTime = 0.0f; // Store the last time
		UINT width = 0;
		UINT height = 0;

		HANDLE fenceEvent = nullptr;
	};
}