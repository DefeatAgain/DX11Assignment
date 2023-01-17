#pragma once
#include"BasicObject.h"
#include"PipeContextSet.h"

/*
	管理一个模型对象，可能是由多个对象组成
*/
template<typename ObjType>
class Object
{
public:
	Object() {}
    Object(const PipeContextSet& pcs):contextSet(pcs){}
    virtual ~Object() {}
	// selfPipe指定是否使用当前模型对象的管道
	void Render(ID3D11DeviceContext* deviceContext, bool selfPipe = true);
	// 生成全部对象的缓冲区
	void SetBuffer(ID3D11Device* device);
	// ObjType决定使用几何体还是加载模型
	ObjType& operator[](size_t index) { return objects[index]; }
protected:
    std::vector<ObjType, AlignedAllocator<ObjType>> objects; // BasicObjcet含有XMMatrix
	PipeContextSet contextSet;
};

template<typename ObjType>
inline void Object<ObjType>::Render(ID3D11DeviceContext* deviceContext, bool selfPipe)
{
	if (selfPipe)
		contextSet.CombineToPipe(deviceContext);
	for (auto beg = objects.begin(); beg != objects.end(); beg++)
	{
		beg->Render(deviceContext);
	}
}

template<typename ObjType>
inline void Object<ObjType>::SetBuffer(ID3D11Device* device)
{
	for (auto beg = objects.begin(); beg != objects.end(); beg++)
	{
		beg->SetBuffer(device);
	}
}
