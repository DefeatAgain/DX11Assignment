#pragma once
#include "Utlity.h"

const D3D11_INPUT_ELEMENT_DESC geoInputLayout[] = 
{
    { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 40, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};

const D3D11_INPUT_ELEMENT_DESC geoSkyCubeInputLayout[] =
{
    { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};


// 基本几何体
struct Geometry
{
    struct VertexData
    {
        DirectX::XMFLOAT3 pos;
        DirectX::XMFLOAT3 normal;
        DirectX::XMFLOAT4 color;
        DirectX::XMFLOAT2 tex;
    };

    struct GeomtryData
    {
        std::vector<VertexData> vertexData; // 顶点数组
        std::vector<unsigned> indexData; // 索引数组
    };
    
    static GeomtryData CreateSphere(float radius = 1.0f, unsigned levels = 20, unsigned slices = 20,
        const DirectX::XMFLOAT4& color = { 0.0f, 0.0f, 0.0f, 0.0f });

    static GeomtryData CreateBox(float width = 2.0f, float height = 2.0f, float depth = 2.0f,
        const DirectX::XMFLOAT4& color = { 0.0f, 0.0f, 0.0f, 0.0f });

    static GeomtryData CreateCylinder(float radius = 1.0f, float height = 2.0f, unsigned slices = 20, unsigned level = 10,
        const DirectX::XMFLOAT4& color = { 0.0f, 0.0f, 0.0f, 0.0f });

    static GeomtryData CreatePlane(float width, float depth, float u, float v,
        const DirectX::XMFLOAT4& color = { 0.0f, 0.0f, 0.0f, 0.0f });
};

