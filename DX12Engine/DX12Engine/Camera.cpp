#include "Camera.h"
#include "InputManager.h"
#define InputInstance InputManager::GetInstance()

using namespace DirectX;
Camera::Camera(float x, float y, float z, float aspectRatio, float moveSpeed)
{
    transform = std::make_shared<Transform>();
    transform->SetPosition(x, y, z);

    this->aspectRatio = aspectRatio;
    this->moveSpeed = moveSpeed;
    tracking = false;

    // calling core methods
    UpdateViewMatrix();
    UpdateProjectionMatrix(this->aspectRatio);
}

Camera::~Camera()
{
}

DirectX::XMFLOAT4X4 Camera::GetViewMatrix()
{
    return viewMat;
}

DirectX::XMFLOAT4X4 Camera::GetProjectionMatrix()
{
    return projMat;
}

DirectX::XMFLOAT3 Camera::GetCameraPosition()
{
    return transform->GetPosition();
}

void Camera::UpdateViewMatrix()
{

    // get the rotation of the camera and store as quaternion
    XMFLOAT3 rotation = transform->GetPitchYawRoll();
    XMVECTOR rotQuat = XMQuaternionRotationRollPitchYawFromVector(XMLoadFloat3(&rotation));

    // create unit forward vector and rotate based on transform
    XMVECTOR unitForward = XMVectorSet(0, 0, 1, 0);
    XMVECTOR actualForward = XMVector3Rotate(unitForward, rotQuat);

    // create/update view matrix with position, direction, and a unit up vector
    XMMATRIX view = XMMatrixLookToLH(
        XMLoadFloat3(&transform->GetPosition()),
        actualForward,
        XMVectorSet(0, 1, 0, 0)
    );

    // apply changes
    XMStoreFloat4x4(&viewMat, view);
}

void Camera::UpdateProjectionMatrix(float aspectRatio)
{
    // update fields
    this->aspectRatio = aspectRatio;

    // create/update projection matrix with FOV, aspect ratio, near and far Z renders
    XMMATRIX proj = XMMatrixPerspectiveFovLH(
        XM_PIDIV4,
        this->aspectRatio,
        0.01f,
        100.0f
    );

    // apply changes
    XMStoreFloat4x4(&projMat, proj);
}

void Camera::Update(float dt)
{
    // InputInstance->Update();
    // check for speed update (sprinting)
    float speed = moveSpeed * dt;
    if (InputInstance->GetKey(Controls::Sprint)) { speed *= 2.0f; }

    // forward and backward movement (relative)
    if (InputInstance->GetKey(Controls::Forward)) { transform->MoveRelative(0, 0, 0.5f * speed); }
    if (InputInstance->GetKey(Controls::Back)) { transform->MoveRelative(0, 0, -0.5f * speed); }
     //left and right (relative)
    if (InputInstance->GetKey(Controls::Left)) { transform->MoveRelative(-0.5f * speed, 0, 0); }
    if (InputInstance->GetKey(Controls::Right)) { transform->MoveRelative(0.5f * speed, 0, 0); }
     // going up (absolute)
    if (InputInstance->GetKey(Controls::Up)) { transform->Move(0, 0.5f * speed, 0); }
     // going down
    if (InputInstance->GetKey(Controls::Down)) { transform->Move(0, -0.5f * speed, 0); }
    //Track mouse
    if (InputInstance->GetKeyPressed(Controls::CameraTracking))
    {
        tracking = !tracking;
        InputInstance->SetLockMouse(tracking);
    }

    // camera rotation
    if (tracking)
    {
        XMFLOAT2 deltaMouse = InputInstance->GetDeltaMouse();

        // rotate transform
        transform->Rotate(-deltaMouse.y * dt * 2, -deltaMouse.x * dt * 2, 0);
    }
    UpdateViewMatrix();
}

std::shared_ptr<Transform> Camera::GetTransform()
{
    return transform;
}
