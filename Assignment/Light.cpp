#include "Light.h"

DirectX::XMMATRIX Light::view = DirectX::XMMatrixIdentity();
DirectX::XMMATRIX Light::ortho = DirectX::XMMatrixIdentity();

Light::Light(const DirectX::XMFLOAT4& carlight1, const DirectX::XMFLOAT4& carlight2, 
	const DirectX::XMFLOAT4& carFront)
{
	using namespace DirectX;
	sunLight.direction = { 500.0f, 500.0f, -500.0f, 0.0f };
	sunLight.ambient = { 0.2f, 0.2f, 0.2f, 0.0f };
	sunLight.diffuse = { 1.0f, 1.0f, 1.0f, 0.0f };
	sunLight.specular = { 1.0f, 1.0f, 1.0f, 0.0f };

	for (unsigned i = 0; i < 2; i++)
	{
		carLight[i].ambient = { 0.0f, 0.0f, 0.0f, 0.0f };
		carLight[i].diffuse = { 1.0f, 1.0f, 1.0f, 0.0f };
		carLight[i].specular = { 1.0f, 1.0f, 1.0f, 0.0f };
		carLight[i].attenuation = { 1.0f, 0.007f, 0.0002f, 0.0f };
		//carLight[i].direction = { 0.0f, 0.0f, -1.0f, 0.0f };
		XMStoreFloat4(&carLight[i].direction, XMVector3Normalize(XMLoadFloat4(&carFront)));
		carLight[i].direction.w = Spot;
	}
	carLight[0].lightPos = carlight1;
	carLight[1].lightPos = carlight2;
	// 计算光源视角相机                    光照向（0，0，0）
	XMVECTOR front_ = XMVector3Normalize(-XMLoadFloat4(&sunLight.direction));
	XMVECTOR right_ = XMVector3Normalize(XMVector3Cross(front_, XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)));
	XMVECTOR up_ = XMVector3Normalize(XMVector3Cross(right_, front_));
	Light::view = XMMatrixLookToLH(XMLoadFloat4(&sunLight.direction), 
		front_, up_);
}

void Light::UpdateLightPos(const DirectX::XMFLOAT4& carlight1, const DirectX::XMFLOAT4& carlight2, 
	const DirectX::XMFLOAT4& carFront)
{
	using namespace DirectX;
	XMStoreFloat4(&carLight[0].direction, XMVector3Normalize(XMLoadFloat4(&carFront)));
	XMStoreFloat4(&carLight[1].direction, XMVector3Normalize(XMLoadFloat4(&carFront)));
	carLight[0].direction.w = Spot;
	carLight[1].direction.w = Spot;
	carLight[0].lightPos = carlight1;
	carLight[1].lightPos = carlight2;
}
