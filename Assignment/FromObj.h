#pragma once
#include "Object.h"

/*
    �������ģ�͵Ĺ�����, ���տ����ƣ�ÿ��ģ��Ӧ���е���һ����
*/
class FromObj :
    public Object<LoadedObject>
{
public:
    FromObj(ID3D11Device* d3dDevice, ID3D11DeviceContext* d3dContext, const PipeContextSet& pcs);
    ~FromObj() override {}
};

