#pragma once
#include "Object.h"

namespace
{
	const float CarSpeed = 5.0f;
	const float Rotation = 15.0f * 0.001f;
	const float WheelRotationSpeed = -1.0f;
	const float SpotDistance = 100.0f;
}

__declspec(align(16))
class Car :
    public Object<GeometryObject>
{
public:
	Car(const PipeContextSet& pcs);
	~Car() override {}
	// ���������ƶ�
	void Move(float dt);
	// ͨ�������ĵ�ǰ��λ��ȡ����
	std::pair<DirectX::XMVECTOR, DirectX::XMVECTOR> GetCarLightPos() const;

	DirectX::XMVECTOR GetCarFront() const { return DirectX::XMLoadFloat4(&Front); }
	DirectX::XMVECTOR GetPosition() const { return DirectX::XMLoadFloat4(&position); }

	bool turnRight = false;
	bool turnLeft = false;
	bool forward = false;
	bool back = false;
private:
	float currentWheelTurnAngle = 0.0f;										//���䳵��ת��Ƕ�
	float totalWheelRotationAngle = 0.0f;									//���䳵�ֹ����Ƕ�
	DirectX::XMFLOAT4 Front = { 0.0f, 0.0f, 1.0f, 0.0f };					//����ǰ������

	std::vector<DirectX::XMMATRIX, AlignedAllocator<DirectX::XMMATRIX>> 
		oriWorldMats;														//������������ʼ����
	std::vector<DirectX::XMMATRIX, AlignedAllocator<DirectX::XMMATRIX>>
		finWorldMats;														//��������������״̬����
	DirectX::XMMATRIX oriFrontWheel1Mat;									//ǰ�ֵĳ�ʼ�������
	DirectX::XMMATRIX oriFrontWheel2Mat;									//ǰ�ֵĳ�ʼ�������

	DirectX::XMFLOAT4 position;												//����λ��
};

