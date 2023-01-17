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

	// 窗口回调函数
	LRESULT WndProcClass(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) override;
	// 加载资源
	void InitResource() override;
	// 准备渲染
	bool LoadContent() override;
	// 停止渲染
	void UnloadContent() override;
	// 帧更新
	void Update(float dt) override;
	// 渲染
	void Render() override;
private:
	Camera camera;
	// 使用shared_ptr是为了使用AlignedAllocator分配对齐的堆内存
	std::shared_ptr<Car> car;
	std::shared_ptr<Plane> plane;
	std::shared_ptr<SkyCube> skyCube;
	std::shared_ptr<Light> lights;
	std::shared_ptr<FromObj> objs;
	std::shared_ptr<ShadowMapping> shadowMap;
private:
	// 更新view和proj矩阵 仅提去了一部分重复代码
	void UpdateViewAndProj();
	void TrackMouse();
};

