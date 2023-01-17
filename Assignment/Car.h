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
	// 处理汽车移动
	void Move(float dt);
	// 通过汽车的当前方位获取汽车
	std::pair<DirectX::XMVECTOR, DirectX::XMVECTOR> GetCarLightPos() const;

	DirectX::XMVECTOR GetCarFront() const { return DirectX::XMLoadFloat4(&Front); }
	DirectX::XMVECTOR GetPosition() const { return DirectX::XMLoadFloat4(&position); }

	bool turnRight = false;
	bool turnLeft = false;
	bool forward = false;
	bool back = false;
private:
	float currentWheelTurnAngle = 0.0f;										//记忆车轮转向角度
	float totalWheelRotationAngle = 0.0f;									//记忆车轮滚动角度
	DirectX::XMFLOAT4 Front = { 0.0f, 0.0f, 1.0f, 0.0f };					//汽车前进方向

	std::vector<DirectX::XMMATRIX, AlignedAllocator<DirectX::XMMATRIX>> 
		oriWorldMats;														//汽车各部件初始矩阵
	std::vector<DirectX::XMMATRIX, AlignedAllocator<DirectX::XMMATRIX>>
		finWorldMats;														//保存汽车各部件状态矩阵
	DirectX::XMMATRIX oriFrontWheel1Mat;									//前轮的初始世界矩阵
	DirectX::XMMATRIX oriFrontWheel2Mat;									//前轮的初始世界矩阵

	DirectX::XMFLOAT4 position;												//汽车位置
};

