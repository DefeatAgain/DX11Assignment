#include"SkyCube.hlsli"
// ������պе�������ɫ��
float4 main(SkyVertexOut sOut) : SV_TARGET
{
    return TexCube.Sample(SamState, sOut.posL);
}