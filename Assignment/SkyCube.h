#pragma once
#include "Object.h"

/*
    ������պе���
*/
class SkyCube :
    public Object<GeometryObject>
{
public:
    SkyCube(ID3D11Device* d3dDevice, ID3D11DeviceContext* d3dContext, float skyRadius);
    ~SkyCube() override;
private:
    ID3D11DepthStencilState* depthState = nullptr; //��պ���Ҫ�Լ�����ȼ��
    ID3D11RasterizerState* rasterizerState = nullptr; //�����رձ����޳�
    ID3D11SamplerState* samplerState = nullptr; // ��������
};

