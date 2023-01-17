#pragma once
#include "Object.h"

/*
    管理天空盒的类
*/
class SkyCube :
    public Object<GeometryObject>
{
public:
    SkyCube(ID3D11Device* d3dDevice, ID3D11DeviceContext* d3dContext, float skyRadius);
    ~SkyCube() override;
private:
    ID3D11DepthStencilState* depthState = nullptr; //天空盒需要自己的深度检测
    ID3D11RasterizerState* rasterizerState = nullptr; //用来关闭背面剔除
    ID3D11SamplerState* samplerState = nullptr; // 采样描述
};

