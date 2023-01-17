#pragma once
#include "Utlity.h"

namespace
{
    const float Spot = 3.0f;
}
/*
    处理光照的类
*/
class Light
{
public:
    struct ParallelLight
    {
        ParallelLight() { ZeroMemory(this, sizeof(ParallelLight)); }
        DirectX::XMFLOAT4 ambient;
        DirectX::XMFLOAT4 diffuse;
        DirectX::XMFLOAT4 specular;
        DirectX::XMFLOAT4 direction;
    };

    struct SpotLight
    {
        SpotLight() { ZeroMemory(this, sizeof(SpotLight)); }
        DirectX::XMFLOAT4 ambient;
        DirectX::XMFLOAT4 diffuse;
        DirectX::XMFLOAT4 specular;
        DirectX::XMFLOAT4 lightPos; // w = range
        DirectX::XMFLOAT4 direction; // w = spot
        DirectX::XMFLOAT4 attenuation;
    };

    struct Material
    {
        Material() { ZeroMemory(this, sizeof(Material)); }
        DirectX::XMFLOAT4 ambient;
        DirectX::XMFLOAT4 diffuse;
        DirectX::XMFLOAT4 specular; // w = 高光范围
    };
public:
    Light(const DirectX::XMFLOAT4& carlight1, const DirectX::XMFLOAT4& carlight2,
        const DirectX::XMFLOAT4& carFront);
    ~Light() {}
    void UpdateLightPos(const DirectX::XMFLOAT4& carlight1, const DirectX::XMFLOAT4& carlight2,
        const DirectX::XMFLOAT4& carFront);

    ParallelLight sunLight; // 平行光（太阳光）
    SpotLight carLight[2]; // 汽车前灯

    const static DirectX::XMMATRIX GetViewMatrix() { return view; }
    const static DirectX::XMMATRIX GetOrthoMatrix() { return ortho; }
    static void XM_CALLCONV SetViewMatrix(DirectX::FXMMATRIX v) { view = v; }
    static void XM_CALLCONV SetOrthoMatrix(DirectX::FXMMATRIX o) { ortho = o; }
private:
    static DirectX::XMMATRIX view;                             // 视图矩阵 
    static DirectX::XMMATRIX ortho;						   // 投影矩阵
};

