#pragma once
#include "pch.h"
struct Vertex {
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 color;

	Vertex(DirectX::XMFLOAT3 position = { 0,0,0 }, DirectX::XMFLOAT3 color = { 1.0f,1.0f,1.0f }) {
		this->position = position;
		this->color = color;
	}

	// do we need a binding description...?

	// We definitely need the input layout though, so here it is
	static std::vector<D3D12_INPUT_ELEMENT_DESC> GetInputLayouts() {
		std::vector<D3D12_INPUT_ELEMENT_DESC> inputLayouts = std::vector<D3D12_INPUT_ELEMENT_DESC>(2);
		
		inputLayouts[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
		inputLayouts[1] = { "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };

		return inputLayouts;
	}
};
