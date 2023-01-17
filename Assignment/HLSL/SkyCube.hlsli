#include "CBuffer.hlsli"

TextureCube TexCube : register(t0);
SamplerState SamState : register(s0);

struct SkyVertexIn
{
    float3 posL : POSITION;
};

struct SkyVertexOut
{
    float4 posW : SV_POSITION;
    float3 posL : POSITIONT;
};