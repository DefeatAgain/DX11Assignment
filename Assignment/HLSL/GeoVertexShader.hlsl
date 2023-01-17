#include "Basics.hlsli"
// 用于几何体的顶点着色器
VertexOut main(VertexIn vIn)
{
    VertexOut vOut;
    vOut.posW = mul(float4(vIn.posL, 1.0f), World);
    matrix ViewProj = mul(View, Proj);
    vOut.posF = mul(vOut.posW, ViewProj);
    vOut.posLS = mul(vOut.posW, toLightSpace);
    vOut.norW = mul(vIn.norL, (float3x3) WorldInvT);
    vOut.color = vIn.color;
    vOut.tex = vIn.tex;
    return vOut;
}