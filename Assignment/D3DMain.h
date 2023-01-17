#pragma once
#include "D3DInitial.h"
#include "Camera.h"
#include "Car.h"
#include "Plane.h"
#include "SkyCube.h"
#include "FromObj.h"
#include "ShadowMapping.h"

class D3DMain :
    public D3DInitial
{
public:
	D3DMain();
	virtual ~D3DMain() override {};

	// ���ڻص�����
	LRESULT WndProcClass(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) override;
	// ������Դ
	void InitResource() override;
	// ׼����Ⱦ
	bool LoadContent() override;
	// ֹͣ��Ⱦ
	void UnloadContent() override;
	// ֡����
	void Update(float dt) override;
	// ��Ⱦ
	void Render() override;
private:
	Camera camera;
	// ʹ��shared_ptr��Ϊ��ʹ��AlignedAllocator�������Ķ��ڴ�
	std::shared_ptr<Car> car;
	std::shared_ptr<Plane> plane;
	std::shared_ptr<SkyCube> skyCube;
	std::shared_ptr<Light> lights;
	std::shared_ptr<FromObj> objs;
	std::shared_ptr<ShadowMapping> shadowMap;
private:
	// ����view��proj���� ����ȥ��һ�����ظ�����
	void UpdateViewAndProj();
	void TrackMouse();
};

