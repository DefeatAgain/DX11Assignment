#pragma once
#include "Light.h"
// ¼Ä´æÆ÷²å²Û
enum register_slot
{
	CBWorld_Slot,
	CBViewEyePos_Slot,
	CBProj_Slot,
	CBLight_Slot,
	CBMaterial_Slot,
	CBToLightSpace_Slot
};

__declspec(align(16)) 
struct CBWorld
{
	DirectX::XMMATRIX world;
	DirectX::XMMATRIX worldInvT;
};

__declspec(align(16)) 
struct CBViewEyePos
{
	DirectX::XMMATRIX view;
	DirectX::XMFLOAT4 eyePos;
};

__declspec(align(16))
struct CBProj
{
	DirectX::XMMATRIX proj;
};

__declspec(align(16)) 
struct CBLight
{
	Light::ParallelLight sunLight;
	Light::SpotLight carLight[2];
};

__declspec(align(16)) 
struct CBMaterial
{
	Light::Material material;
};

__declspec(align(16))
struct CBToLightSpace
{
	DirectX::XMMATRIX toLightSpace;
};
