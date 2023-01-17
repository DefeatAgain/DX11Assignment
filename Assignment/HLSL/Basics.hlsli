#include "CBuffer.hlsli"

struct VertexIn
{
    float3 posL : POSITION;
    float3 norL : NORMAL;
    float4 color : COLOR;
    float2 tex : TEXCOORD;
};

struct VertexOut
{
    float4 posF : SV_POSITION;
    float4 posW : POSITION; // 光照计算
    float3 norW : NORMAL; // 光照计算  
    float4 posLS : POSITION1; // 阴影计算
    float4 color : COLOR;
    float2 tex : TEXCOORD;
};

struct ObjVertexIn
{
    float3 posL : POSITION;
    float3 norL : NORMAL;
    float3 tanL : TANGENT;
    float2 tex : TEXCOORD;
};

struct ObjVertexOut
{
    float4 posF : SV_POSITION;
    float4 posW : POSITION; // 光照计算
    float3 norW : NORMAL; // 光照计算 
    float4 posLS : POSITION1; // 阴影计算
    float3 tanW : TANGENT;
    float2 tex : TEXCOORD;
};

Texture2D Tex_Diff : register(t0);
Texture2D Tex_Spec : register(t1);
Texture2D Tex_Nor : register(t2);
Texture2D Tex_Depth : register(t3);
SamplerState SamLinear : register(s0);
SamplerState SamShadow : register(s1);

float3 ComputeTBNnormal(float3 norS, float3 tanW, float3 norW)
{
    // 还原法线至切线空间
    float3 norT = 2.0f * norS - 1.0f;
    
    // 构建TBN矩阵
    float3 N = norW;
    float3 T = normalize(tanW - dot(tanW, N) * N);
    float3 B = cross(N, T);
    
    float3x3 TBN = float3x3(T, B, N);
    
    // 还原法线
    return mul(norT, TBN);
}

float ComputeShadow(float4 lightSpacePos, float3 lightDir, float3 normal)
{
    static const float DIV_TEXTURE_SIZE = 1.0f / 1024.0f;
    static const float DIV_SAMPLE_NUM = 1.0f / 9.0f;
    // 透视除法
    float3 perspective = lightSpacePos.xyz / lightSpacePos.w;
    // 从NDC还原至[0, 1]空间中
    perspective.x = perspective.x * 0.5f + 0.5f;
    perspective.y = perspective.y * -0.5f + 0.5f; // y轴需要反转
    // 将视锥体之外的点排除
    if (perspective.z > 1.0f)
        return 0.0f;

    float cur_dept = perspective.z;
    // 修正阴影失真
    float bias = max(0.005f * (1.0f - dot(normal, normalize(lightDir))), 0.0005f);
    // PCF
    float shadow = 0.0f;
    perspective.xy -= DIV_TEXTURE_SIZE;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            float2 incr = float2(i, j) * DIV_TEXTURE_SIZE;
            float shadow_dept = Tex_Depth.Sample(SamShadow, perspective.xy + incr).r;
            shadow += cur_dept - bias > shadow_dept ? 1.0f : 0.0f;
        }
    }
    return shadow * DIV_SAMPLE_NUM;
}