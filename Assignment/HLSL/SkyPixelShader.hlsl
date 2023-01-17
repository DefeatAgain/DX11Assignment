#include"SkyCube.hlsli"
// 用于天空盒的像素着色器
float4 main(SkyVertexOut sOut) : SV_TARGET
{
    return TexCube.Sample(SamState, sOut.posL);
}