#pragma once
#include "pch.h"

struct PipelineStateStream
{
    CD3DX12_PIPELINE_STATE_STREAM_ROOT_SIGNATURE pRootSignature;
    CD3DX12_PIPELINE_STATE_STREAM_INPUT_LAYOUT InputLayout;
    CD3DX12_PIPELINE_STATE_STREAM_PRIMITIVE_TOPOLOGY PrimitiveTopologyType;
    CD3DX12_PIPELINE_STATE_STREAM_VS VS;
    CD3DX12_PIPELINE_STATE_STREAM_PS PS;
    CD3DX12_PIPELINE_STATE_STREAM_DEPTH_STENCIL_FORMAT DSVFormat;
    CD3DX12_PIPELINE_STATE_STREAM_RENDER_TARGET_FORMATS RTVFormats;
};
class Material
{

    
protected:

	// Wide string arguments to the shader paths
	LPCWSTR vertexShaderPath;
	LPCWSTR fragmentShaderPath;
    PipelineStateStream pipelineStateStream;
    // Pipeline state object.
    Microsoft::WRL::ComPtr<ID3D12PipelineState> m_PipelineState;

    // Layouts for the vertex inputs
    std::vector<D3D12_INPUT_ELEMENT_DESC> inputLayouts;
    // Root signature
    Microsoft::WRL::ComPtr<ID3D12RootSignature> m_RootSignature;


    // Depth buffer.
    Microsoft::WRL::ComPtr<ID3D12Resource> m_DepthBuffer;
    // Descriptor heap for depth buffer.
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_DSVHeap;


public:
    
    Material(LPCWSTR vertexShaderPath, LPCWSTR fragmentShaderPath);

    /// <summary>
    /// Creates and allocates Material resources
    /// </summary>
    void Init();

    /// <summary>
    /// Creates and allocates the graphics pipeline (PSO) used by this material
    /// </summary>
    void CreateGraphicsPipeline();

    /// <summary>
    /// Sets the vertex input attributes for the input layout
    /// </summary>
    /// <param name="inputs"></param>
    void SetupVertexInput(std::vector<D3D12_INPUT_ELEMENT_DESC> inputs);

    // Resize the depth buffer to match the size of the client area.
    void ResizeDepthBuffer(int width, int height);

#pragma region Accessor
    /// <summary>
    /// Returns the Pipeline State Object used by this material (the pipeline itself)
    /// </summary>
    /// <returns></returns>
    Microsoft::WRL::ComPtr<ID3D12PipelineState> GetPSO();

    /// <summary>
    /// Gets the root signature used to create the pipeline
    /// </summary>
    /// <returns></returns>
    Microsoft::WRL::ComPtr<ID3D12RootSignature> GetRootSignature();
#pragma endregion
};

