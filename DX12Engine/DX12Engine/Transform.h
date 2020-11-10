#pragma once
#include "pch.h"
class Transform
{
private:
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 pitchYawRoll;
	DirectX::XMFLOAT3 scale;

	DirectX::XMMATRIX orientation;

	// World matrix
	bool matrixDirty;
	DirectX::XMFLOAT4X4 worldMatrix;

public:
	Transform();
	void Move(float x, float y, float z);
	void Rotate(float p, float y, float r);
	void Scale(float x, float y, float z);

	void SetPosition(float x, float y, float z);
	void SetPitchYawRoll(float p, float y, float r);
	void SetScale(float x, float y, float z);

	DirectX::XMFLOAT3 GetPosition();
	DirectX::XMFLOAT3 GetPitchYawRoll();
	DirectX::XMFLOAT3 GetScale();

	DirectX::XMFLOAT4X4 GetWorldMatrix();

	void MoveRelative(float x, float y, float z);
	void MoveRelative(DirectX::XMFLOAT3 moveDir);

	// Rotates about a given axis by the provided degrees
	void RotateAboutAxis(DirectX::XMVECTOR axis, float degrees);
};

