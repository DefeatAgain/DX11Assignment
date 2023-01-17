#include "Basics.hlsli"
// 用于加载模型的像素着色器
float4 main(ObjVertexOut vOut) : SV_TARGET
{
    float4 initColor = Tex_Diff.Sample(SamLinear, vOut.tex);
    clip(initColor.a - 0.1f); // 丢弃alpha通道小于0.1的像素
    
    float4 specColor = Tex_Spec.Sample(SamLinear, vOut.tex);
    float3 norS = normalize(Tex_Nor.Sample(SamLinear, vOut.tex).r);
    float3 bumpNor = ComputeTBNnormal(norS, vOut.tanW, vOut.norW);

    float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 specular = float4(0.0f, 0.0f, 0.0f, 0.0f);
    
    float4 Amb = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 Diff = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 Spec = float4(0.0f, 0.0f, 0.0f, 0.0f);
    
    float shadow = ComputeShadow(vOut.posLS, SunLight.Direction.xyz, vOut.norW);
    ComputeParalleLight(initColor, initColor, specColor, SunLight, bumpNor, EyePos.xyz, 
        vOut.posW.xyz, Amb, Diff, Spec);
    ambient += Amb;
    diffuse += (1.0f - shadow) * Diff;
    specular += (1.0f - shadow) * Spec;
    
    for (int i = 0; i < 2; i++)
    {
        ComputeSpotLight(initColor, initColor, specColor, CarLight[i], bumpNor, EyePos.xyz,
            vOut.posW.xyz, Amb, Diff, Spec);
        ambient += Amb;
        diffuse += Diff;
        specular += Spec;
    }

    return initColor * (diffuse + specular + ambient);
}