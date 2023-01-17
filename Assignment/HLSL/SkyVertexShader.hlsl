#include"SkyCube.hlsli"
// ������պеĶ�����ɫ��
SkyVertexOut main(SkyVertexIn sIn)
{
    SkyVertexOut sOut;
    sOut.posL = sIn.posL;
    float3x3 view = (float3x3)View;
    sOut.posW = float4(mul(sIn.posL, view), 1.0f);
    sOut.posW = mul(sOut.posW, Proj);
    sOut.posW = sOut.posW.xyww; // ������պ���Զƽ��
    return sOut;
}