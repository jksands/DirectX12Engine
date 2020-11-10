#include "Mesh.h"
#include "pch.h"
#include "Vertex.h"
#include "Material.h"
#include "Application.h"
// eventually Transform.h
using namespace DirectX;
Mesh::Mesh(std::shared_ptr<Material> material, std::vector<Vertex> vertices, std::vector<uint16_t> indices)
{
	this->material = material;
	this->vertices = vertices;
	this->indices = indices;

    activeInstanceCount = 0;


}

void Mesh::Init()
{
	CreateVertexBuffer();
	CreateIndexBuffer();
}

void Mesh::CreateVertexBuffer()
{
}

void Mesh::CreateIndexBuffer()
{
}

void Mesh::UpdateVertexBuffer()
{
}

void Mesh::UpdateIndexBuffer()
{
}

void Mesh::UpdateBufferResource(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList2> commandList, ID3D12Resource** pDestinationResource, ID3D12Resource** pIntermediateResource, size_t numElements, size_t elementSize, const void* bufferData, D3D12_RESOURCE_FLAGS flags)
{
    auto device = Application::Get().GetDevice();

    size_t bufferSize = numElements * elementSize;
    // Create a committed resource for the GPU resource in a default heap.
    ThrowIfFailed(device->CreateCommittedResource(
        &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
        D3D12_HEAP_FLAG_NONE,
        &CD3DX12_RESOURCE_DESC::Buffer(bufferSize, flags),
        D3D12_RESOURCE_STATE_COPY_DEST,
        nullptr,
        IID_PPV_ARGS(pDestinationResource)));
    // Create an committed resource for the upload.
    if (bufferData)
    {
        ThrowIfFailed(device->CreateCommittedResource(
            &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
            D3D12_HEAP_FLAG_NONE,
            &CD3DX12_RESOURCE_DESC::Buffer(bufferSize),
            D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr,
            IID_PPV_ARGS(pIntermediateResource)));
        D3D12_SUBRESOURCE_DATA subresourceData = {};
        subresourceData.pData = bufferData;
        subresourceData.RowPitch = bufferSize;
        subresourceData.SlicePitch = subresourceData.RowPitch;

        UpdateSubresources(commandList.Get(),
            *pDestinationResource, *pIntermediateResource,
            0, 0, 1, &subresourceData);
    }
}

void Mesh::UploadData()
{
    auto commandQueue = Application::Get().GetCommandQueue(D3D12_COMMAND_LIST_TYPE_COPY);
    auto commandList = commandQueue->GetCommandList();

    // Upload vertex buffer data.
    ComPtr<ID3D12Resource> intermediateVertexBuffer;
    UpdateBufferResource(commandList.Get(),
        &m_VertexBuffer, &intermediateVertexBuffer,
        vertices.size(), sizeof(Vertex), vertices.data());

    // Create the vertex buffer view.
    m_VertexBufferView.BufferLocation = m_VertexBuffer->GetGPUVirtualAddress();
    m_VertexBufferView.SizeInBytes = vertices.size() * sizeof(Vertex);
    m_VertexBufferView.StrideInBytes = sizeof(Vertex);

    // Upload index buffer data.
    ComPtr<ID3D12Resource> intermediateIndexBuffer;
    UpdateBufferResource(commandList.Get(),
        &m_IndexBuffer, &intermediateIndexBuffer,
        indices.size(), sizeof(WORD), indices.data());
    // Create index buffer view.
    m_IndexBufferView.BufferLocation = m_IndexBuffer->GetGPUVirtualAddress();
    m_IndexBufferView.Format = DXGI_FORMAT_R16_UINT;
    m_IndexBufferView.SizeInBytes = indices.size() * sizeof(WORD);

    auto fenceValue = commandQueue->ExecuteCommandList(commandList);
    commandQueue->WaitForFenceValue(fenceValue);
}

Microsoft::WRL::ComPtr<ID3D12Resource> Mesh::GetVertexBuffer()
{
    return m_VertexBuffer;
}

Microsoft::WRL::ComPtr<ID3D12Resource> Mesh::GetIndexBuffer()
{
	return m_IndexBuffer;
}

D3D12_VERTEX_BUFFER_VIEW Mesh::GetVertexBufferView()
{
    return m_VertexBufferView;
}

D3D12_INDEX_BUFFER_VIEW Mesh::GetIndexBufferView()
{
    return m_IndexBufferView;
}

std::vector<WORD> Mesh::GetIndices()
{
    return indices;
}

int Mesh::AddInstance(std::shared_ptr<Transform> t)
{
    activeInstanceCount++;
    instances.push_back(t);
    return activeInstanceCount - 1;
}

void Mesh::RemoveInstance(int instanceId)
{
    instances.erase(instances.begin() + instanceId);
    activeInstanceCount--;
}

uint32_t Mesh::GetInstanceCount()
{
    return activeInstanceCount;
}

std::shared_ptr<Transform> Mesh::GetInstanceTransform(int index)
{
    return instances[index];
}

void Mesh::GenerateCube(DirectX::XMFLOAT3 color)
{
    vertices.resize(8);
    vertices = {
        { XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f) }, // 0
        { XMFLOAT3(-1.0f,  1.0f, -1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f) }, // 1
        { XMFLOAT3(1.0f,  1.0f, -1.0f), XMFLOAT3(1.0f, 1.0f, 0.0f) }, // 2
        { XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f) }, // 3
        { XMFLOAT3(-1.0f, -1.0f,  1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f) }, // 4
        { XMFLOAT3(-1.0f,  1.0f,  1.0f), XMFLOAT3(0.0f, 1.0f, 1.0f) }, // 5
        { XMFLOAT3(1.0f,  1.0f,  1.0f), XMFLOAT3(1.0f, 1.0f, 1.0f) }, // 6
        { XMFLOAT3(1.0f, -1.0f,  1.0f), XMFLOAT3(1.0f, 0.0f, 1.0f) }  // 7
    };

    indices = {
    0, 1, 2, 0, 2, 3,
    4, 6, 5, 4, 7, 6,
    4, 5, 1, 4, 1, 0,
    3, 2, 6, 3, 6, 7,
    1, 5, 6, 1, 6, 2,
    4, 0, 3, 4, 3, 7
    };

    UploadData();
}
