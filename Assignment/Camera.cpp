#include "Camera.h"

void Camera::ProcessMouseMovement(float xoffset, float yoffset)
{
    if (!third) // 第一人称不需要更新摄像机方向
        return;

    xoffset *= SENSITIVITY;
    yoffset *= SENSITIVITY;

    yaw += xoffset;
    pitch += yoffset;

    // 防止摄像机反转
    if (pitch > 1.0f)
        pitch = 1.0f;
    else if (pitch < -89.0f)
        pitch = -89.0f;

    // 防止过度偏航
    if (yaw > 270.0f)
        yaw = 270.0f;
    else if (yaw < -90.0f)
        yaw = -90.0f;

    updateCameraVectors();
}

void Camera::ProcessMouseScroll(float yoffset)
{
    zoom -= (float)yoffset;
    if (zoom < 45.0f)
        zoom = 45.0f;
    if (zoom > 90.0f)
        zoom = 90.0f;
}

void Camera::ProcessViewChange()
{
    if (third)
    {
        third = false;
    }
    else
    {
        third = true;
    }
}

DirectX::XMMATRIX Camera::GetViewMatrix()
{
    using namespace DirectX;
    if (third)
    {
        XMStoreFloat4(&Position, XMVector3Transform(XMLoadFloat4(&ObjectPos),
            XMMatrixTranslationFromVector(-30.0f * XMLoadFloat4(&Front))));
        return XMMatrixLookToLH(XMLoadFloat4(&Position), XMLoadFloat4(&Front), XMLoadFloat4(&Up));
    }
    else
    {
        XMStoreFloat4(&Position, XMVector3Transform(XMLoadFloat4(&ObjectPos), 
            XMMatrixTranslationFromVector(10.0f * XMLoadFloat4(&ObjectFront))));
        return XMMatrixLookToLH(XMLoadFloat4(&Position), XMLoadFloat4(&ObjectFront),
            XMLoadFloat4(&WORLDUP));
    }
}

void Camera::updateCameraVectors()
{
    using namespace DirectX;
    XMFLOAT4 front = { 0.0f, 0.0f, 0.0f, 0.0f };
    // 欧拉角计算与起始坐标轴有关,这里选取x轴
    front.x = cosf(XMConvertToRadians(yaw)) * cosf(XMConvertToRadians(pitch));
    front.y = sinf(XMConvertToRadians(pitch));
    front.z = sinf(XMConvertToRadians(yaw)) * cosf(XMConvertToRadians(pitch));
    XMStoreFloat4(&Front, XMVector3Normalize(XMLoadFloat4(&front)));
    DirectX::XMVECTOR Right = XMVector4Normalize(XMVector3Cross(XMLoadFloat4(&front), 
        XMLoadFloat4(&WORLDUP)));
    XMStoreFloat4(&Up, XMVector3Normalize(XMVector3Cross(Right, XMLoadFloat4(&front))));
}
