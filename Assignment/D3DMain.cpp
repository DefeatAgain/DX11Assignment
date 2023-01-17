#include "D3DMain.h"
#include "Shader.h"
#include "ConstantStruct.h"

namespace
{
    D3DMain* pD3DMain = nullptr; // 用于定位窗口响应函数
	// 跟踪鼠标
	bool firstMouse = true;
	float lastX;
	float lastY;
}

enum VK_CAR
{
	VK_W = 0x57,
	VK_S = 0x53,
	VK_D = 0x44,
	VK_A = 0x41,
	VK_V = 0x56
};

D3DMain::D3DMain()
{
    pD3DMain = this;
}

LRESULT D3DMain::WndProcClass(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		EndPaint(hwnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_SIZE:
		if (!d3dDevice)
			break;
		if (wParam != SIZE_MINIMIZED)
			ReSize();
		break;
	case WM_MOUSELEAVE:
		firstMouse = true;
		break;
	case WM_MOUSEMOVE:
	{
		TrackMouse(); // 跟踪鼠标离开事件
		//POINT point({ LOWORD(lParam),HIWORD(lParam) });
		POINTS point = MAKEPOINTS(lParam);
		//ScreenToClient(hwnd, &point);
		if (point.x < 0 || point.y < 0)
			break;
		float xpos = point.x;
		float ypos = point.y;
		if (firstMouse)
		{
			lastX = xpos;
			lastY = ypos;
			firstMouse = false;
		}
		float xoffset = xpos - lastX;
		float yoffset = lastY - ypos; //反转y轴

		lastX = xpos;
		lastY = ypos;

		camera.ProcessMouseMovement(xoffset, yoffset);
		break;
	}
	case WM_MOUSEWHEEL:
	{
		float yOffset = GET_WHEEL_DELTA_WPARAM(wParam) * 0.1f; // 降低灵敏度
		camera.ProcessMouseScroll(yOffset);
		break;
	}
	case WM_KEYDOWN: // 开始汽车移动
	{
		switch ((UINT)wParam)
		{
		case VK_W:
			car->forward = true; break;
		case VK_S:
			car->back = true; break;
		case VK_A:
			car->turnLeft = true; break;
		case VK_D:
			car->turnRight = true; break;
		default:
			break;
		}
		break;
	}
	case WM_KEYUP: // 停止汽车移动
	{
		switch ((UINT)wParam)
		{
		case VK_W:
			car->forward = false; break;
		case VK_S:
			car->back = false; break;
		case VK_A:
			car->turnLeft = false; break;
		case VK_D:
			car->turnRight = false; break;
		case VK_V:
			camera.ProcessViewChange(); break;
		default:
			break;
		}
		break;
	}
	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
	return 0;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    return pD3DMain->WndProcClass(hwnd, message, wParam, lParam);
}

void D3DMain::InitResource()
{
	using namespace DirectX;

	assert(d3dDevice);
	assert(d3dContext);
	// 共享管道设定
	PipeContextSet sharedContext(d3dDevice);
	PipeContextSet::CreateConstantBuffer(d3dDevice, d3dContext);

	// 初始化shader
	Shader pubShader(L"HLSL\\GeoVertexShader.hlsl", L"HLSL\\GeoPixelShader.hlsl");
	//Shader objShader(L"HLSL\\ObjVertexShader.hlsl", L"HLSL\\ObjPixelShader.hlsl");
	pubShader.CreateShaderAndLayout(d3dDevice, geoInputLayout);
	sharedContext.SetShaderAndLayout(pubShader.vs, pubShader.ps, pubShader.il);
	// 初始化场景
	car = std::allocate_shared<Car, AlignedAllocator<Car>>(AlignedAllocator<Car>(), sharedContext);
	plane = std::make_shared<Plane>(d3dDevice, d3dContext, sharedContext);
	skyCube = std::make_shared<SkyCube>(d3dDevice, d3dContext, 1000.0f);
	objs = std::make_shared<FromObj>(d3dDevice, d3dContext, sharedContext);
	shadowMap = std::make_shared<ShadowMapping>(d3dDevice);
	// 设置各自所需缓冲区
	car->SetBuffer(d3dDevice);
	plane->SetBuffer(d3dDevice);
	skyCube->SetBuffer(d3dDevice);
	objs->SetBuffer(d3dDevice);
}

bool D3DMain::LoadContent()
{
	using namespace DirectX;
	// 初始化缓存
	InitResource();

	assert(car);
	assert(plane);
	assert(objs);
	assert(skyCube);

	// 保存汽车位置
	XMStoreFloat4(&camera.ObjectPos, car->GetPosition());
	XMStoreFloat4(&camera.ObjectFront, car->GetCarFront());

	// 设置view和proj
	PipeContextSet::SetViewMatrix(XMMatrixTranspose(camera.GetViewMatrix()));
	PipeContextSet::SetProjMatrix(XMMatrixTranspose(XMMatrixPerspectiveFovLH(XM_PIDIV2,
		(float)width / (float)height, 1.0f, 1000.0f)));

	// 初始化灯光
	auto carLights = car->GetCarLightPos();
	XMFLOAT4 carLight1, carLight2, lightFront;
	XMStoreFloat4(&carLight1, carLights.first);
	XMStoreFloat4(&carLight2, carLights.second);
	lightFront = camera.ObjectFront;
	lights = std::make_shared<Light>(carLight1, carLight2, lightFront);
	// 变换至光照空间所需矩阵
	XMMATRIX lightView = Light::GetViewMatrix();
	XMMATRIX lightProj = XMMatrixOrthographicLH(150.0f, 150.0f, 800.0f, 1000.0f);
	Light::SetOrthoMatrix(lightProj);

	CBLight cbLight;
	for (int i = 0; i < 2; i++)
		cbLight.carLight[i] = lights->carLight[i];
	cbLight.sunLight = lights->sunLight;

	CBToLightSpace cbToLightSpace;
	cbToLightSpace.toLightSpace = XMMatrixTranspose(lightView * lightProj);
	// 更新缓冲区
	ID3D11Buffer* cBuffer = nullptr;
	d3dContext->PSGetConstantBuffers(CBLight_Slot, 1, &cBuffer);
	CopyConstantData(d3dContext, cBuffer, &cbLight);
	SafeRelease(cBuffer);
	d3dContext->VSGetConstantBuffers(CBToLightSpace_Slot, 1, &cBuffer);
	CopyConstantData(d3dContext, cBuffer, &cbToLightSpace);
	SafeRelease(cBuffer);
	UpdateViewAndProj();
	return true;
}

void D3DMain::UnloadContent()
{
	PipeContextSet::ReleaseConstantBuffer();
	car.reset();
	plane.reset();
	skyCube.reset();
	objs.reset();
}

void D3DMain::Update(float dt)
{
	using namespace DirectX;
	// 汽车移动所需变换
	car->Move(dt);
	// 保存汽车位置
	XMStoreFloat4(&camera.ObjectPos, car->GetPosition());
	XMStoreFloat4(&camera.ObjectFront, car->GetCarFront());
	// 更新view和proj矩阵
	PipeContextSet::SetViewMatrix(XMMatrixTranspose(camera.GetViewMatrix()));
	PipeContextSet::SetProjMatrix(XMMatrixTranspose(XMMatrixPerspectiveFovLH(XMConvertToRadians(
		camera.zoom), (float)width / (float)height, 1.0f, 1000.0f)));

	// 显示阴影区域会随汽车变化，此做法会让阴影边缘跳动
	// 更好的办法是移动光源和光源照射点（保持方向不变）
	XMMATRIX lightView = Light::GetViewMatrix();
	float objectPosZ = camera.ObjectPos.z;
	if (objectPosZ > 0.0f)
		objectPosZ = -objectPosZ;
	XMMATRIX lightProj = XMMatrixOrthographicLH(
		150.0f - objectPosZ, 150.0f - objectPosZ,
		800.0f + objectPosZ, 1200.0f + objectPosZ);
	Light::SetOrthoMatrix(lightProj);

	// 更新汽车前灯
	auto carLights = car->GetCarLightPos();
	XMFLOAT4 carLight1, carLight2, lightFront;
	XMStoreFloat4(&carLight1, carLights.first);
	XMStoreFloat4(&carLight2, carLights.second);
	lightFront = camera.ObjectFront;
	lights->UpdateLightPos(carLight1, carLight2, lightFront);
	CBLight cbLight;
	for (int i = 0; i < 2; i++)
		cbLight.carLight[i] = lights->carLight[i];
	cbLight.sunLight = lights->sunLight;

	CBToLightSpace cbToLightSpace;
	cbToLightSpace.toLightSpace = XMMatrixTranspose(lightView * lightProj);
	// 更新常量缓冲区
	ID3D11Buffer* cBuffer = nullptr;
	d3dContext->PSGetConstantBuffers(CBLight_Slot, 1, &cBuffer);
	CopyConstantData(d3dContext, cBuffer, &cbLight);
	SafeRelease(cBuffer);
	d3dContext->VSGetConstantBuffers(CBToLightSpace_Slot, 1, &cBuffer);
	CopyConstantData(d3dContext, cBuffer, &cbToLightSpace);
	SafeRelease(cBuffer);

	UpdateViewAndProj();
}

void D3DMain::Render()
{
	// 清空渲染缓冲区
	float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	d3dContext->ClearRenderTargetView(backBufferTarget, clearColor);
	d3dContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	// 开始阴影渲染
	shadowMap->RenderStart(d3dContext);
	plane->Render(d3dContext, false);
	objs->Render(d3dContext, false);
	car->Render(d3dContext, false);
	shadowMap->RenderEnd(d3dContext);
	// 开始正常渲染
	plane->Render(d3dContext);
	objs->Render(d3dContext);
	car->Render(d3dContext);
	skyCube->Render(d3dContext);
	swapChain->Present(0, 0);
}

void D3DMain::UpdateViewAndProj()
{
	using namespace DirectX;

	XMMATRIX view =  PipeContextSet::GetViewMatrix();
	XMMATRIX proj = PipeContextSet::GetProjMatrix();

	CBViewEyePos cbEyePos;
	cbEyePos.view = view;
	cbEyePos.eyePos = camera.Position;

	CBProj cbProj;
	cbProj.proj = proj;

	ID3D11Buffer* cBuffer = nullptr;
	d3dContext->VSGetConstantBuffers(CBViewEyePos_Slot, 1, &cBuffer);
	CopyConstantData(d3dContext, cBuffer, &cbEyePos);
	SafeRelease(cBuffer);
	d3dContext->PSGetConstantBuffers(CBViewEyePos_Slot, 1, &cBuffer);
	CopyConstantData(d3dContext, cBuffer, &cbEyePos);
	SafeRelease(cBuffer);
	d3dContext->VSGetConstantBuffers(CBProj_Slot, 1, &cBuffer);
	CopyConstantData(d3dContext, cBuffer, &cbProj);
	SafeRelease(cBuffer);
}

void D3DMain::TrackMouse()
{
	TRACKMOUSEEVENT tme;
	tme.cbSize = sizeof(tme);
	tme.hwndTrack = hwnd;
	tme.dwFlags = TME_LEAVE;
	tme.dwHoverTime = 1;
	TrackMouseEvent(&tme);
}
