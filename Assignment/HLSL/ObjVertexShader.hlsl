#include "Basics.hlsli"
// 用于加载模型的顶点着色器
ObjVertexOut main(ObjVertexIn vIn)
{
    ObjVertexOut vOut;
    vOut.posW = mul(float4(vIn.posL, 1.0f), World);
    matrix ViewProj = mul(View, Proj);
    vOut.posF = mul(vOut.posW, ViewProj);
    vOut.posLS = mul(vOut.posW, toLightSpace);
    vOut.norW = mul(vIn.norL, (float3x3) WorldInvT);
    vOut.tanW = mul(float4(vIn.tanL, 1.0f), World).xyz;
    vOut.tex = vIn.tex;
    return vOut;
}