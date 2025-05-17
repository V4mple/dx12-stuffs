#include "pch.h"
#include "Renderer.h"

namespace Psycho
{
    Renderer::~Renderer() {
        Release();
    }

    void Renderer::Initialize(HWND hwnd, UINT width, UINT height) {
        this->width = width;
        this->height = height;

        DX12Debug::Get().Enable(); // Enable debug layer for better diagnostics

        DXGIFactory factory;
        DXGIAdapter adapter = factory.GetAdapter();

        DXGI_ADAPTER_DESC desc;
        adapter.Get()->GetDesc(&desc);
        PRINT_W_N("Selected Device: " << desc.Description);

        device.Initialize(adapter.Get());
        commandQueue.Initialize(device.Get());
        commandList.Initialize(device.Get());
        swapChain.Initialize(device.Get(), factory.Get(), commandQueue.Get(), hwnd, width, height);

        if (objectLoader.LoadObjFile("Assets/cocacola.obj")) // Replace with actual path
        {
            vertices = objectLoader.GetVertices();
            indices = objectLoader.GetIndices();
        }
        else
        {
            std::cerr << "Failed to load model!" << std::endl;
        }

        indexCount = static_cast<UINT>(indices.size());
        /*
                vertices = {
            {{-0.5f, -0.5f,  0.5f}, {1.0f, 0.0f, 0.0f, 1.0f}},
            {{ 0.5f, -0.5f,  0.5f}, {0.0f, 1.0f, 0.0f, 1.0f}},
            {{ 0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 1.0f, 1.0f}},
            {{-0.5f,  0.5f,  0.5f}, {1.0f, 1.0f, 0.0f, 1.0f}},
            {{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 1.0f, 1.0f}},
            {{ 0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 1.0f, 1.0f}},
            {{ 0.5f,  0.5f, -0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}},
            {{-0.5f,  0.5f, -0.5f}, {0.5f, 0.5f, 0.5f, 1.0f}}
        };

        indices = {
            0, 1, 2,  2, 3, 0,  // Front
            5, 4, 7,  7, 6, 5,  // Back
            4, 0, 3,  3, 7, 4,  // Left
            1, 5, 6,  6, 2, 1,  // Right
            3, 2, 6,  6, 7, 3,  // Top
            4, 5, 1,  1, 0, 4   // Bottom
        };
        */

        dynamicVertexBuffer.Initialize(device.Get(), sizeof(Vertex) * vertices.size(), D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATE_GENERIC_READ);
        dynamicIndexBuffer.Initialize(device.Get(), sizeof(uint32_t) * indices.size(), D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATE_GENERIC_READ);

        memcpy(dynamicVertexBuffer.GetCPUMemory(), vertices.data(), sizeof(Vertex) * vertices.size());
        memcpy(dynamicIndexBuffer.GetCPUMemory(), indices.data(), sizeof(uint32_t) * indices.size());

        // Update buffer views
        dynamicVertexBufferView.BufferLocation = dynamicVertexBuffer.Get()->GetGPUVirtualAddress();
        dynamicVertexBufferView.StrideInBytes = sizeof(Vertex);
        dynamicVertexBufferView.SizeInBytes = sizeof(Vertex) * vertices.size();

        dynamicIndexBufferView.BufferLocation = dynamicIndexBuffer.Get()->GetGPUVirtualAddress();
        dynamicIndexBufferView.Format = DXGI_FORMAT_R32_UINT;
        dynamicIndexBufferView.SizeInBytes = sizeof(uint32_t) * indices.size();

        // Depth buffer setup
        D3D12_DESCRIPTOR_HEAP_DESC depthHeapDesc = {};
        depthHeapDesc.NumDescriptors = 1;
        depthHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
        depthHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
        EVALUATE_HR(device.Get()->CreateDescriptorHeap(&depthHeapDesc, IID_PPV_ARGS(depthHeap.GetAddressOf())), "Failed to create depth heap");

        D3D12_HEAP_PROPERTIES heapProps = { D3D12_HEAP_TYPE_DEFAULT };

        D3D12_RESOURCE_DESC depthDesc = {};
        depthDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
        depthDesc.Width = width;
        depthDesc.Height = height;
        depthDesc.DepthOrArraySize = 1;
        depthDesc.MipLevels = 1;
        depthDesc.Format = DXGI_FORMAT_D32_FLOAT;
        depthDesc.SampleDesc = { 1, 0 };
        depthDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
        D3D12_CLEAR_VALUE depthClear = { DXGI_FORMAT_D32_FLOAT, { 1.0f, 0 } };
        EVALUATE_HR(device.Get()->CreateCommittedResource(&heapProps, D3D12_HEAP_FLAG_NONE, &depthDesc, D3D12_RESOURCE_STATE_DEPTH_WRITE, &depthClear, IID_PPV_ARGS(depthBuffer.GetAddressOf())), "Failed to create depth buffer");

        D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
        dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
        dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
        device.Get()->CreateDepthStencilView(depthBuffer.Get(), &dsvDesc, depthHeap->GetCPUDescriptorHandleForHeapStart());

        pipeline.Initialize(device.Get());

        viewPort.TopLeftX = 0;
        viewPort.TopLeftY = 0;
        viewPort.Width = (float)width;
        viewPort.Height = (float)height;
        viewPort.MinDepth = 0.0f;
        viewPort.MaxDepth = 1.0f;

        dx12Rect.left = 0;
        dx12Rect.right = viewPort.Width;
        dx12Rect.top = 0;
        dx12Rect.bottom = viewPort.Height;

        constantBufferPassData.Initialize(device.Get(), Utilities::CalculateConstantBufferAlignment(sizeof(PassData)), D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATE_GENERIC_READ);
    }

    void Renderer::UpdateDraw(float dt) {
        cubeRotationAngle += 0.5f * dt; // Uncomment if you want rotation back
        DirectX::XMMATRIX worldMatrix = DirectX::XMMatrixRotationY(cubeRotationAngle);

        // Camera looks at a point offset from its position
        DirectX::XMVECTOR eye = DirectX::XMVectorSet(cameraPosition.x, cameraPosition.y, cameraPosition.z, 0.0f);
        DirectX::XMVECTOR target = DirectX::XMVectorAdd(
            eye,
            DirectX::XMVectorSet(cameraTargetOffset.x, cameraTargetOffset.y, cameraTargetOffset.z, 0.0f)
        );
        viewProjectionMatrix = DirectX::XMMatrixLookAtLH(
            eye,
            target,
            DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
        ) * DirectX::XMMatrixPerspectiveFovLH(
            DirectX::XM_PIDIV4,
            static_cast<float>(width) / static_cast<float>(height),
            0.1f, 1000.0f
        );

        struct PassData {
            DirectX::XMMATRIX world;
            DirectX::XMMATRIX viewProj;
        } passData;
        passData.world = worldMatrix;
        passData.viewProj = viewProjectionMatrix;
        memcpy(constantBufferPassData.GetCPUMemory(), &passData, sizeof(PassData));

        D3D12_RESOURCE_BARRIER barrier = {};
        barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
        barrier.Transition.pResource = swapChain.GetCurrentRenderTarget();
        barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
        barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
        commandList.gpuCommand()->ResourceBarrier(1, &barrier);

        const float clearColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
        D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = swapChain.GetCurrentHandle();
        D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = depthHeap->GetCPUDescriptorHandleForHeapStart();
        commandList.gpuCommand()->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
        commandList.gpuCommand()->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
        commandList.gpuCommand()->OMSetRenderTargets(1, &rtvHandle, false, &dsvHandle);

        commandList.gpuCommand()->RSSetViewports(1, &viewPort);
        commandList.gpuCommand()->RSSetScissorRects(1, &dx12Rect);

        commandList.gpuCommand()->SetGraphicsRootSignature(pipeline.GetRootSignature());
        commandList.gpuCommand()->SetPipelineState(pipeline.Get());
        commandList.gpuCommand()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

        commandList.gpuCommand()->IASetVertexBuffers(0, 1, &dynamicVertexBufferView);
        commandList.gpuCommand()->IASetIndexBuffer(&dynamicIndexBufferView);
        commandList.gpuCommand()->SetGraphicsRootConstantBufferView(0, constantBufferPassData.Get()->GetGPUVirtualAddress());

        commandList.gpuCommand()->DrawIndexedInstanced(indexCount, 1, 0, 0, 0);

        barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
        barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
        commandList.gpuCommand()->ResourceBarrier(1, &barrier);

        commandList.gpuCommand()->Close();
        commandQueue.ExecuteCommandList(commandList.Get());
        swapChain.Present();

        static HANDLE fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
        commandQueue.GetFence()->SetEventOnCompletion(commandQueue.GetCurrentFenceValue(), fenceEvent);
        WaitForSingleObject(fenceEvent, INFINITE);
        commandList.ResetCommandList();
    }

    void Renderer::Release() {
        constantBufferPassData.Release();
        commandQueue.FlushQueue();
        pipeline.Release();
        dynamicVertexBuffer.Release();
        dynamicIndexBuffer.Release();
        commandList.Release();
        swapChain.Release();
        depthBuffer.Reset();
        depthHeap.Reset();
        commandQueue.ReleaseQueue();
        if (device.Get()) {
            device.Reset();
        }
        CloseHandle(fenceEvent);
    }
}