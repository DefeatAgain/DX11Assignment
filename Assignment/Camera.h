#pragma once
#include<DirectXMath.h>

namespace
{
	const float YAW = 90.0f; // ŷ������ʼ���������
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

	// ��������ƶ�
	void ProcessMouseMovement(float xoffset, float yoffset);
	// ����������
	void ProcessMouseScroll(float yoffset);
	// �����ӽ��л�
	void ProcessViewChange();
	// ��ȡview����
	DirectX::XMMATRIX GetViewMatrix();
private:
	// ŷ����
	float yaw = YAW;
	float pitch = PITCH;
	// �Ƿ�����˳�
	bool third = true;
	// ͨ��ŷ���Ǽ����ӽǷ���
	void updateCameraVectors();
};

