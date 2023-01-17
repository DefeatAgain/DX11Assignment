#pragma once
#include"BasicObject.h"
#include"PipeContextSet.h"

/*
	����һ��ģ�Ͷ��󣬿������ɶ���������
*/
template<typename ObjType>
class Object
{
public:
	Object() {}
    Object(const PipeContextSet& pcs):contextSet(pcs){}
    virtual ~Object() {}
	// selfPipeָ���Ƿ�ʹ�õ�ǰģ�Ͷ���Ĺܵ�
	void Render(ID3D11DeviceContext* deviceContext, bool selfPipe = true);
	// ����ȫ������Ļ�����
	void SetBuffer(ID3D11Device* device);
	// ObjType����ʹ�ü����廹�Ǽ���ģ��
	ObjType& operator[](size_t index) { return objects[index]; }
protected:
    std::vector<ObjType, AlignedAllocator<ObjType>> objects; // BasicObjcet����XMMatrix
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
