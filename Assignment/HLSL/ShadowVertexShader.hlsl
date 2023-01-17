#include "CBuffer.hlsli"

float4 main( float3 pos : POSITION ) : SV_POSITION
{
    float4 posW = mul(float4(pos, 1.0f), World);
    return mul(posW, toLightSpace);
}