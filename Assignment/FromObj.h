#pragma once
#include "Object.h"

/*
    用与加载模型的管理类, 按照框架设计，每个模型应该有单独一个类
*/
class FromObj :
    public Object<LoadedObject>
{
public:
    FromObj(ID3D11Device* d3dDevice, ID3D11DeviceContext* d3dContext, const PipeContextSet& pcs);
    ~FromObj() override {}
};

