#pragma once
#include "pch.h"
#include "Vertex.h"
#include "Material.h"
#include "CommandQueue.h"
#include "Transform.h"
class Mesh
{
private:
    // Vertices
    std::vector<Vertex> vertices;
    Microsoft::WRL::ComPtr<ID3D12Resource> m_VertexBuffer;
    D3D12_VERTEX_BUFFER_VIEW m_VertexBufferView;

    // Indices
    std::vector<WORD> indices;
    Microsoft::WRL::ComPtr<ID3D12Resource> m_IndexBuffer;
    D3D12_INDEX_BUFFER_VIEW m_IndexBufferView;

    // Instances  (future)
    std::vector<std::shared_ptr<Transform>> instances;
    uint32_t activeInstanceCount;
    // Material
    std::shared_ptr<Material> material;

    bool instanceBufferDirty = true;

public:
#pragma region Constructor

    Mesh(
        std::shared_ptr<Material> material = nullptr,
        std::vector<Vertex> vertices = {},
        std::vector<WORD> indices = {}
    );
#pragma endregion

#pragma region BufferStuff
    /// <summary>
    /// Creates and allocates mesh resources
    /// </summary>
    void Init();

    /// <summary>
    /// Creates and allocates the instance buffer that will be used by this mesh
    /// </summary>
    // void CreateInstanceBuffer();

    /// <summary>
    /// Creates and allocates the vertex buffer that will be used by this mesh
    /// </summary>
    void CreateVertexBuffer();

    /// <summary>
    /// Creates and allocates the index buffer that will be used by this mesh
    /// </summary>
    void CreateIndexBuffer();

    /// <summary>
    /// Updates the mesh's vertex buffer 
    /// </summary>
    void UpdateVertexBuffer();

    /// <summary>
    /// Updates the mesh's index buffer
    /// </summary>
    void UpdateIndexBuffer();

    // Create a GPU buffer. NOTE: This will likely go in its OWN file in the future
    void UpdateBufferResource(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList2> commandList,
        ID3D12Resource** pDestinationResource, ID3D12Resource** pIntermediateResource,
        size_t numElements, size_t elementSize, const void* bufferData,
        D3D12_RESOURCE_FLAGS flags = D3D12_RESOURCE_FLAG_NONE);

    void UploadData();
    // void UploadIndexData();
#pragma endregion

#pragma region Accessors
    Microsoft::WRL::ComPtr<ID3D12Resource> GetVertexBuffer();
    Microsoft::WRL::ComPtr<ID3D12Resource> GetIndexBuffer();

    D3D12_VERTEX_BUFFER_VIEW GetVertexBufferView();
    D3D12_INDEX_BUFFER_VIEW GetIndexBufferView();
    std::vector<WORD> GetIndices();
    int AddInstance(std::shared_ptr<Transform> t);
    void RemoveInstance(int instanceId);
    uint32_t GetInstanceCount();
    std::shared_ptr<Transform> GetInstanceTransform(int index);
#pragma endregion
    
#pragma region Meshes
    void GenerateCube(DirectX::XMFLOAT3 color = { 1.0f, 1.0f, 1.0f });
#pragma endregion
};

