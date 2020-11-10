#pragma once
#include "pch.h";
#include "Transform.h";

class Camera
{
public:
	Camera(float x, float y, float z, float aspectRatio, float moveSpeed);
	~Camera();

	DirectX::XMFLOAT4X4 GetViewMatrix();
	DirectX::XMFLOAT4X4 GetProjectionMatrix();

	DirectX::XMFLOAT3 GetCameraPosition();

	void UpdateViewMatrix();
	void UpdateProjectionMatrix(float aspectRatio);

	void Update(float dt);

	std::shared_ptr<Transform> GetTransform();

private:
	std::shared_ptr<Transform> transform;
	DirectX::XMFLOAT4X4 viewMat;
	DirectX::XMFLOAT4X4 projMat;

	// customization fields
	float aspectRatio;
	float moveSpeed;
	bool tracking;

};