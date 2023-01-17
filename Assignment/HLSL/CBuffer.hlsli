#include "Light.hlsli"

cbuffer CBWorld : register(b0)
{
    matrix World;
    matrix WorldInvT;
}

cbuffer CBViewEyePos : register(b1)
{
    matrix View;
    float4 EyePos;
}

cbuffer CBProj : register(b2)
{
    matrix Proj;
}

cbuffer CBLight : register(b3)
{
    ParallelLight SunLight;
    SpotLight CarLight[2];
}

cbuffer CBMaterial : register(b4)
{
    Material material;
}

cbuffer CBLightSpace : register(b5)
{
    matrix toLightSpace;
}