#include "Basics.hlsli"
// 用于几何体的像素着色器
float4 main(VertexOut vOut) : SV_TARGET
{
    float4 initColor =  Tex_Diff.Sample(SamLinear, vOut.tex) + vOut.color;
    
    float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 specular = float4(0.0f, 0.0f, 0.0f, 0.0f);
    
    float4 Amb = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 Diff = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 Spec = float4(0.0f, 0.0f, 0.0f, 0.0f);
    
    float shadow = ComputeShadow(vOut.posLS, SunLight.Direction.xyz, vOut.norW);
    // 太阳光
    ComputeParalleLight(material, SunLight, vOut.norW, EyePos.xyz, vOut.posW.xyz, Amb, Diff, Spec);
    ambient += Amb;
    diffuse += (1.0f - shadow) * Diff;
    specular += (1.0f - shadow) * Spec;
    // 汽车前灯    
    for (int i = 0; i < 2; i++)
    {
        ComputeSpotLight(material, CarLight[i], vOut.norW, EyePos.xyz, vOut.posW.xyz, Amb, Diff, Spec);
        ambient += Amb;
        diffuse += Diff;
        specular += Spec;
    }

    return initColor * (diffuse + specular + ambient);
}