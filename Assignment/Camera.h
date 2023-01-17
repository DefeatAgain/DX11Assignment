#pragma once
#include<DirectXMath.h>

namespace
{
	const float YAW = 90.0f; // 欧拉角起始坐标轴决定
	const float PITCH = -20.0f;
	const float SENSITIVITY = 0.5f;
	const float ZOOM = 90.0f;

	const DirectX::XMFLOAT4 WORLDUP = { 0.0f, 1.0f, 0.0f, 0.0f };
	const DirectX::XMFLOAT4 INV_WORLDUP = { 0.0f, -1.0f, 0.0f, 0.0f };
}

class Camera
{
public:
	DirectX::XMFLOAT4 Position = {0.0f, 0.0f, -5.0f, 1.0f};
	DirectX::XMFLOAT4 Front = {0.0f, 0.0f, 1.0f, 0.0f};
	DirectX::XMFLOAT4 Up = {0.0f, 1.0f, 0.0f, 1.0f};

	DirectX::XMFLOAT4 ObjectPos = {0.0f, 0.0f, 0.0f, 1.0f};
	DirectX::XMFLOAT4 ObjectFront = {0.0f, 0.0f, 0.0f, 0.0f};
	// fov
	float zoom = ZOOM;

	// 处理鼠标移动
	void ProcessMouseMovement(float xoffset, float yoffset);
	// 处理鼠标滚轮
	void ProcessMouseScroll(float yoffset);
	// 处理视角切换
	void ProcessViewChange();
	// 获取view矩阵
	DirectX::XMMATRIX GetViewMatrix();
private:
	// 欧拉角
	float yaw = YAW;
	float pitch = PITCH;
	// 是否第三人称
	bool third = true;
	// 通过欧拉角计算视角方向
	void updateCameraVectors();
};

