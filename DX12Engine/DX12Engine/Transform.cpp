#include "Transform.h"

using namespace DirectX;

Transform::Transform()
{
	// set up initial transform data
	position = XMFLOAT3(0, 0, 0);
	pitchYawRoll = XMFLOAT3(0, 0, 0);
	scale = XMFLOAT3(1, 1, 1);

	XMStoreFloat4x4(&worldMatrix, XMMatrixIdentity());
	orientation = XMMatrixIdentity();
	matrixDirty = false;
}

void Transform::Move(float x, float y, float z)
{
	position.x += x; position.y += y; position.z += z;
	matrixDirty = true;
}

void Transform::Rotate(float p, float y, float r)
{
	pitchYawRoll.x += p; pitchYawRoll.y += y; pitchYawRoll.z += r;
	matrixDirty = true;
}

void Transform::Scale(float x, float y, float z)
{
	scale.x *= x; scale.y *= y; scale.z *= z;
	matrixDirty = true;
}

void Transform::SetPosition(float x, float y, float z) { position.x = x;		position.y = y;		position.z = z;		matrixDirty = true; }
void Transform::SetPitchYawRoll(float p, float y, float r) { 
	pitchYawRoll.x = p;	pitchYawRoll.y = y;	pitchYawRoll.z = r;	
	orientation = XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&pitchYawRoll));
	matrixDirty = true; 
}
void Transform::SetScale(float x, float y, float z) { scale.x = x;			scale.y = y;		scale.z = z;		matrixDirty = true; }

DirectX::XMFLOAT3 Transform::GetPosition()
{
	return position;
}

DirectX::XMFLOAT3 Transform::GetPitchYawRoll()
{
	return pitchYawRoll;
}

DirectX::XMFLOAT3 Transform::GetScale()
{
	return scale;
}

DirectX::XMFLOAT4X4 Transform::GetWorldMatrix()
{
	if (matrixDirty) // matrix needs to be updated
	{
		matrixDirty = false;

		// Create three transformation components
		XMMATRIX trans = XMMatrixTranslationFromVector(XMLoadFloat3(&position));
		// XMMATRIX rot = XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&pitchYawRoll));
		XMMATRIX sc = XMMatrixScalingFromVector(XMLoadFloat3(&scale));

		// Combine and store
		XMMATRIX wm = sc * orientation * trans;
		XMStoreFloat4x4(&worldMatrix, wm);
	}
	return worldMatrix;
}

void Transform::MoveRelative(float x, float y, float z)
{
	// Adjust current position of the transform by specified amounts
	//		relative to the transform's rotation.
	// To help, get the values into XMVECTOR using XMVectorSet()
	XMVECTOR movement = XMVectorSet(x, y, z, 0);
	XMVECTOR quat = XMQuaternionRotationRollPitchYaw(pitchYawRoll.x, pitchYawRoll.y, pitchYawRoll.z);

	// Now have absolute direction and rotation, time to rotate the direction
	XMVECTOR dirMove = XMVector3Rotate(movement, quat);

	// load and store
	XMStoreFloat3(&position, XMLoadFloat3(&position) + dirMove);
	matrixDirty = true;
}

void Transform::MoveRelative(DirectX::XMFLOAT3 moveDir)
{
	// Simply call the other method (this is for convenience)
	MoveRelative(moveDir.x, moveDir.y, moveDir.z);
}

void Transform::RotateAboutAxis(XMVECTOR axis, float degrees)
{
	// apply rotation to current matrix
	orientation = XMMatrixRotationAxis(axis, XMConvertToRadians(degrees));
	matrixDirty = true;
}

