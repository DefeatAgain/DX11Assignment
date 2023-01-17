#pragma once
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "D3DCompiler.lib")
#pragma comment(lib, "assimp-vc142-mt.lib")

#include "Utlity.h"

// 窗口响应回调
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

namespace
{
	const unsigned WIDTH = 800U;
	const unsigned HEIGHT = 600U;
}

class D3DInitial
{
public:
	D3DInitial() {}
	virtual ~D3DInitial() {}
	// 初始化D3D
	bool Initialize(HINSTANCE hinstance, HWND hwnd);
	// 退出D3D
	void Shutdown();
	// 窗口回调函数
	virtual LRESULT WndProcClass(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) = 0;
	// 加载资源
	virtual void InitResource() = 0;
	// 准备渲染
	virtual bool LoadContent() = 0;
	// 停止渲染
	virtual void UnloadContent() = 0;
	// 帧更新
	virtual void Update(float dt) = 0;
	// 开始渲染
	virtual void Render() = 0;

protected:
	// 窗口大小改变
	void UpdateWindowSize();
	void ReSize();
protected:
	HINSTANCE hInstance = nullptr;
	HWND hwnd = nullptr;

	D3D_DRIVER_TYPE driverType = D3D_DRIVER_TYPE_NULL;
	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;

	ID3D11Device* d3dDevice = nullptr;
	ID3D11DeviceContext* d3dContext = nullptr;
	IDXGISwapChain* swapChain = nullptr;
	ID3D11RenderTargetView* backBufferTarget = nullptr;
	ID3D11Texture2D* depthStencilBuffer = nullptr;
	ID3D11DepthStencilView* depthStencilView = nullptr;
	UINT MSAA4xQuality = 0;

	unsigned int width = WIDTH;
	unsigned int height = HEIGHT;
};

