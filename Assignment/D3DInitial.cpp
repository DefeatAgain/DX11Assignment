#include "D3DInitial.h"

bool D3DInitial::Initialize(HINSTANCE hinstance, HWND hwnd)
{
	this->hInstance = hInstance;
	this->hwnd = hwnd;

	RECT dimensions;
	GetClientRect(hwnd, &dimensions);

	unsigned width = dimensions.right - dimensions.left;
	unsigned height = dimensions.bottom - dimensions.top;
	// 驱动类型
	D3D_DRIVER_TYPE driverTypes[] = {
		D3D_DRIVER_TYPE_HARDWARE, D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE, D3D_DRIVER_TYPE_SOFTWARE
	};
	unsigned totalDriverTypes = ARRAYSIZE(driverTypes);
	// 特性等级
	D3D_FEATURE_LEVEL featureLevels[] = {
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
	};
	unsigned totalFeatureLevels = ARRAYSIZE(featureLevels);

	unsigned int creationFlags = 0;
#ifdef _DEBUG
	creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	HRESULT hr;
	unsigned int driver = 0;
	for (driver = 0; driver < totalDriverTypes; driver++)
	{
		hr = D3D11CreateDevice(nullptr, driverTypes[driver], nullptr, creationFlags,
			featureLevels, totalFeatureLevels,
			D3D11_SDK_VERSION, &d3dDevice, &featureLevel, &d3dContext);
		if (SUCCEEDED(hr))
		{
			driverType = driverTypes[driver];
			break;
		}
	}
	CheckHR(hr);

	// 检测 MSAA支持的质量等级
	d3dDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &MSAA4xQuality);
	assert(MSAA4xQuality > 0);

	// 仅尝试创建d3d11
	IDXGIDevice* dxgiDevice = nullptr;
	IDXGIAdapter* dxgiAdapter = nullptr;
	IDXGIFactory1* dxgiFactory1 = nullptr;

	CheckHR(d3dDevice->QueryInterface(&dxgiDevice));
	CheckHR(dxgiDevice->GetAdapter(&dxgiAdapter));
	CheckHR(dxgiAdapter->GetParent(__uuidof(IDXGIFactory1),	reinterpret_cast<void**>(&dxgiFactory1)));

	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = width;
	swapChainDesc.BufferDesc.Height = height;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.OutputWindow = hwnd;
	swapChainDesc.Windowed = true;
	swapChainDesc.SampleDesc.Count = 4;
	swapChainDesc.SampleDesc.Quality = MSAA4xQuality - 1;

	CheckHR(dxgiFactory1->CreateSwapChain(d3dDevice, &swapChainDesc, &swapChain));
	SafeRelease(dxgiDevice);
	SafeRelease(dxgiAdapter);
	SafeRelease(dxgiFactory1);

	ReSize();
	return LoadContent();
}

void D3DInitial::Shutdown()
{
	UnloadContent();

	SafeRelease(backBufferTarget);
	SafeRelease(depthStencilView);
	SafeRelease(swapChain);
	SafeRelease(d3dContext);
	SafeRelease(d3dDevice);
	SafeRelease(depthStencilBuffer);
}

void D3DInitial::ReSize()
{
	assert(d3dContext);
	assert(d3dDevice);
	assert(swapChain);

	SafeRelease(backBufferTarget);
	SafeRelease(depthStencilView);
	SafeRelease(depthStencilBuffer);

	UpdateWindowSize();

	// 重设交换链并且重新创建渲染目标视图
	HRESULT result;
	result = swapChain->ResizeBuffers(1, width, height, DXGI_FORMAT_B8G8R8A8_UNORM, 0);
	CheckHR(result);
	// 帧缓冲
	ID3D11Texture2D* backBufferTexture;
	result = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferTexture);
	CheckHR(result);

	result = d3dDevice->CreateRenderTargetView(backBufferTexture, 0, &backBufferTarget);
	SafeRelease(backBufferTexture);
	CheckHR(result);

	D3D11_TEXTURE2D_DESC depthStencilDesc;
	depthStencilDesc.Width = width;
	depthStencilDesc.Height = height;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = 4;
	depthStencilDesc.SampleDesc.Quality = MSAA4xQuality - 1;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;
	result = d3dDevice->CreateTexture2D(&depthStencilDesc, nullptr, &depthStencilBuffer);
	CheckHR(result);
	// 创建深度缓冲区以及深度模板视图
	result = d3dDevice->CreateDepthStencilView(depthStencilBuffer, nullptr, &depthStencilView);
	CheckHR(result);
	// 将渲染目标视图和深度模板缓冲区结合到管线
	d3dContext->OMSetRenderTargets(1, &backBufferTarget, depthStencilView);

	D3D11_VIEWPORT viewport;
	viewport.Width = static_cast<float>(width);
	viewport.Height = static_cast<float>(height);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
	d3dContext->RSSetViewports(1, &viewport);
}

void D3DInitial::UpdateWindowSize()
{
	RECT dimensions;
	GetClientRect(hwnd, &dimensions);
	width = dimensions.right - dimensions.left;
	height = dimensions.bottom - dimensions.top;
}
