#pragma once
#include "Object.h"
/*
	用于创建地面的类
*/
class Plane:
	public Object<GeometryObject>
{
public:
	Plane(ID3D11Device* d3dDevice, ID3D11DeviceContext* d3dContext, const PipeContextSet& pcs);
	~Plane() override {}
};

