#include "Car.h"
#include "Geometry.h"

Car::Car(const PipeContextSet& pcs)
	:Object(pcs)
{
	using namespace DirectX;
	//Car bottom
	GeometryObject bottom(Geometry::CreateBox(6.0f, 1.0f, 10.0f, XMFLOAT4(23.0f/255.0f, 124.0f/255.0f, 176.0f/255.0f,1.0f)));
	//Car front/rear
	GeometryObject front(Geometry::CreateBox(6.0f, 2.0f, 1.0f, XMFLOAT4(23.0f/255.0f, 124.0f/255.0f, 176.0f/255.0f, 1.0f)));
	GeometryObject rear(Geometry::CreateBox(6.0f, 2.0f, 1.0f, XMFLOAT4(23.0f/255.0f, 124.0f/255.0f, 176.0f/255.0f, 1.0f)));
	//Car medium
	GeometryObject medium(Geometry::CreateBox(6.0f, 2.0f, 4.0f, XMFLOAT4(23.0f/255.0f, 124.0f/255.0f, 176.0f/255.0f, 1.0f)));
	//Car top
	GeometryObject top1(Geometry::CreateBox(6.0f, 1.4f, 3.0f, XMFLOAT4(175.0f/255.0f, 221.0f/255.0f, 34.0f/255.0f, 1.0f)));
	GeometryObject top2(Geometry::CreateBox(6.0f, 2.0f, 3.0f, XMFLOAT4(175.0f/255.0f, 221.0f/255.0f, 34.0f/255.0f, 1.0f)));
	GeometryObject top3(Geometry::CreateBox(6.0f, 1.4f, 7.0f, XMFLOAT4(175.0f/255.0f, 221.0f/255.0f, 34.0f/255.0f, 1.0f)));
	GeometryObject top4(Geometry::CreateBox(6.0f, 1.4f, 4.5f, XMFLOAT4(175.0f/255.0f, 221.0f/255.0f, 34.0f/255.0f, 1.0f)));
	GeometryObject top5(Geometry::CreateBox(6.0f, 1.0f, 4.8f, XMFLOAT4(175.0f/255.0f, 221.0f/255.0f, 34.0f/255.0f, 1.0f)));
	//Car wheel
	GeometryObject wheel1(Geometry::CreateCylinder(1.0f, 1.0f, 10, 10, XMFLOAT4(57.0f/255.0f, 47.0f/255.0f, 65.0f/255.0f, 1.0f)));
	GeometryObject wheel2(Geometry::CreateCylinder(1.0f, 1.0f, 10, 10, XMFLOAT4(57.0f/255.0f, 47.0f/255.0f, 65.0f/255.0f, 1.0f)));
	GeometryObject wheel3(Geometry::CreateCylinder(1.0f, 1.0f, 10, 10, XMFLOAT4(57.0f/255.0f, 47.0f/255.0f, 65.0f/255.0f, 1.0f)));
	GeometryObject wheel4(Geometry::CreateCylinder(1.0f, 1.0f, 10, 10, XMFLOAT4(57.0f/255.0f, 47.0f/255.0f, 65.0f/255.0f, 1.0f)));
	//Car Light
	GeometryObject light1(Geometry::CreateCylinder(0.3f, 1.0f, 10, 5, XMFLOAT4(1.0f, 199.0f/255.0f, 155.0f/255.0f, 1.0f)));
	GeometryObject light2(Geometry::CreateCylinder(0.3f, 1.0f, 10, 5, XMFLOAT4(1.0f, 199.0f/255.0f, 155.0f/255.0f, 1.0f)));
	//assemble
	XMMATRIX scale = XMMatrixScaling(1.5f, 1.5f, 1.5f);
	oriWorldMats.emplace_back(XMMatrixTranslation(0.0f, 1.5f, 0.0f) * scale);
	oriWorldMats.emplace_back(XMMatrixTranslation(0.0f, 1.0f, 4.5f) * scale);
	oriWorldMats.emplace_back(XMMatrixTranslation(0.0f, 1.0f, 0.0f) * scale);
	oriWorldMats.emplace_back(XMMatrixTranslation(0.0f, 1.0f, -4.5f) * scale);
	oriWorldMats.emplace_back(XMMatrixTranslation(0.0f, 2.7f, 3.5f) * scale);
	oriWorldMats.emplace_back(XMMatrixRotationX(XMConvertToRadians(45.0f)) * XMMatrixTranslation(0.0f, 3.7f, 0.2f) * scale);
	oriWorldMats.emplace_back(XMMatrixTranslation(0.0f, 2.7f, -1.5f) * scale);
	oriWorldMats.emplace_back(XMMatrixTranslation(0.0f, 4.0f, -2.75f) * scale);
	oriWorldMats.emplace_back(XMMatrixTranslation(0.0f, 4.9f, -2.6f) * scale);
	oriWorldMats.emplace_back(XMMatrixRotationZ(XMConvertToRadians(90.0f)) * XMMatrixTranslation(-2.5f, 0.0f, 3.0f) * scale);
	oriWorldMats.emplace_back(XMMatrixRotationZ(XMConvertToRadians(90.0f)) * XMMatrixTranslation(2.5f, 0.0f, 3.0f) * scale);
	oriWorldMats.emplace_back(XMMatrixRotationZ(XMConvertToRadians(90.0f)) * XMMatrixTranslation(-2.5f, 0.0f, -3.0f) * scale);
	oriWorldMats.emplace_back(XMMatrixRotationZ(XMConvertToRadians(90.0f)) * XMMatrixTranslation(2.5f, 0.0f, -3.0f) * scale);
	oriWorldMats.emplace_back(XMMatrixRotationX(XMConvertToRadians(90.0f)) * XMMatrixTranslation(-1.8f, 1.3f, 4.6f) * scale);
	oriWorldMats.emplace_back(XMMatrixRotationX(XMConvertToRadians(90.0f)) * XMMatrixTranslation(1.8f, 1.3f, 4.6f) * scale);

	Light::Material carBody;
	carBody.ambient = { 0.70754f, 0.70754f, 0.70754f, 0.0f };
	carBody.diffuse = { 0.70754f, 0.70754f, 0.70754f, 0.0f };
	carBody.specular = { 0.608273f, 0.608273f, 0.608273f, 256.0f };
	Light::Material carWheel;
	carWheel.ambient = { 0.09f, 0.09f, 0.09f, 0.0f };
	carWheel.diffuse = { 0.41f, 0.41f, 0.41f, 0.0f };
	carWheel.specular = { 0.10f, 0.10f, 0.10f, 32.0f };
	bottom.SetMaterial(carBody);
	front.SetMaterial(carBody);
	medium.SetMaterial(carBody);
	rear.SetMaterial(carBody);
	top1.SetMaterial(carBody);
	top2.SetMaterial(carBody);
	top3.SetMaterial(carBody);
	top4.SetMaterial(carBody);
	top5.SetMaterial(carBody);
	light1.SetMaterial(carBody);
	light2.SetMaterial(carBody);
	wheel1.SetMaterial(carWheel);
	wheel2.SetMaterial(carWheel);
	wheel3.SetMaterial(carWheel);
	wheel4.SetMaterial(carWheel);

	objects.push_back(std::move(bottom));
	objects.push_back(std::move(front));
	objects.push_back(std::move(medium));
	objects.push_back(std::move(rear));
	objects.push_back(std::move(top1));
	objects.push_back(std::move(top2));
	objects.push_back(std::move(top3));
	objects.push_back(std::move(top4));
	objects.push_back(std::move(top5));
	objects.push_back(std::move(wheel1));
	objects.push_back(std::move(wheel2));
	objects.push_back(std::move(wheel3));
	objects.push_back(std::move(wheel4));
	objects.push_back(std::move(light1));
	objects.push_back(std::move(light2));
	
	position = { 0.0f, 1.0f, 0.0f, 1.0f };//只加入y分量 防止汽车旋转导致汽车坐标出现错误
	oriFrontWheel1Mat = oriWorldMats[9];
	oriFrontWheel2Mat = oriWorldMats[10];
	//finWorldMats.insert(finWorldMats.end(), oriWorldMats.begin(), oriWorldMats.end()); //最终world矩阵
	finWorldMats.resize(objects.size(), XMMatrixIdentity());
	for (size_t i = 0; i < objects.size(); i++)
		objects[i].SetWorldMatrix(oriWorldMats[i]);
}

void Car::Move(float dt)
{
	using namespace DirectX;
	// 车轮转向效果
	if (turnRight)
		currentWheelTurnAngle = 30.0f;
	else if (turnLeft)
		currentWheelTurnAngle = -30.0f;
	else
		currentWheelTurnAngle = 0.0f;
	// 车轮转向矩阵
	XMMATRIX wheelTurnMat = XMMatrixRotationX(XMConvertToRadians(currentWheelTurnAngle));
	// 汽车只旋转轮胎而不前进
	if (!forward && !back)
	{
		oriWorldMats[9] = wheelTurnMat * oriFrontWheel1Mat;
		oriWorldMats[10] = wheelTurnMat * oriFrontWheel2Mat;
		objects[9].SetWorldMatrix(oriWorldMats[9] * finWorldMats[9]);
		objects[10].SetWorldMatrix(oriWorldMats[10] * finWorldMats[10]);
		return;
	}
	// 汽车位移
	float carOffset = CarSpeed * dt;
	float rotation;
	float wheelRotation = WheelRotationSpeed;
	if (turnLeft)
		rotation = -Rotation;
	else if (turnRight)
		rotation = Rotation;
	else
		rotation = 0.0f;

	if (back)
	{
		carOffset = -carOffset;
		rotation = -rotation;
		wheelRotation = -wheelRotation;
	}
	totalWheelRotationAngle += wheelRotation;
	// 限制轮胎前后转动角度
	if (totalWheelRotationAngle > 360.0f)
		totalWheelRotationAngle = 1.0f;
	else if (totalWheelRotationAngle < -360.0f)
		totalWheelRotationAngle = -1.0f;

	// 汽车移动效果
	XMMATRIX offsetMat = XMMatrixTranslationFromVector(carOffset * XMLoadFloat4(&Front));
	XMMATRIX rotationMat = XMMatrixRotationY(XMConvertToRadians(rotation));
	// 轮胎旋转效果
	XMMATRIX wheelRotationMat = XMMatrixRotationY(XMConvertToRadians(totalWheelRotationAngle));
	wheelTurnMat = XMMatrixRotationX(XMConvertToRadians(currentWheelTurnAngle));
	oriWorldMats[9] = wheelRotationMat * wheelTurnMat * oriFrontWheel1Mat;
	oriWorldMats[10] = wheelRotationMat * wheelTurnMat * oriFrontWheel2Mat;
	// 无需为后轮单独保存初始世界
	oriWorldMats[11] = wheelRotationMat * oriWorldMats[11];
	oriWorldMats[12] = wheelRotationMat * oriWorldMats[12];

	for (size_t i = 0; i < objects.size(); i++)
	{					 //汽车整体转向   //汽车之前的状态     //汽车位移
		finWorldMats[i] = rotationMat * finWorldMats[i] * offsetMat;
		objects[i].SetWorldMatrix(oriWorldMats[i] * finWorldMats[i]);// 更新汽车的世界矩阵
	}
	// 更新汽车方位
	XMStoreFloat4(&position, XMVector4Transform(XMLoadFloat4(&position), offsetMat));
	XMStoreFloat4(&Front, XMVector4Transform(XMLoadFloat4(&Front), rotationMat));
}

std::pair<DirectX::XMVECTOR, DirectX::XMVECTOR> Car::GetCarLightPos() const
{
	using namespace DirectX;
	XMVECTOR Front_ = XMLoadFloat4(&Front);
	XMVECTOR Position_ = XMLoadFloat4(&position);
	XMMATRIX frontOffset = XMMatrixTranslationFromVector(8.0f * Front_);
	XMVECTOR right = XMVector3Cross(Front_, XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
	XMVECTOR left = XMVector3Cross(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), Front_);
	XMVECTOR light1 = XMVector3Transform(Position_, frontOffset *
		XMMatrixTranslationFromVector(2.5f * right));
	XMVECTOR light2 = XMVector3Transform(Position_, frontOffset *
		XMMatrixTranslationFromVector(2.5f * left));
	return { XMVectorSetW(light1, SpotDistance), XMVectorSetW(light2, SpotDistance) };// 设置聚光灯照射距离
}
