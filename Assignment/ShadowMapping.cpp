#include "ShadowMapping.h"
#include "ConstantStruct.h"
#include "Light.h"
#include "Shader.h"
#include "Geometry.h"

ShadowMapping::ShadowMapping(ID3D11Device* d3dDevice)
{
	assert(d3dDevice);

	HRESULT result;

	ID3D11Texture2D* textureBuffer = nullptr;
	D3D11_TEXTURE2D_DESC textureDesc;
	textureDesc.Width = SHADOW_WIDTH;
	textureDesc.Height = SHADOW_HEIGHT;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;
	result = d3dDevice->CreateTexture2D(&textureDesc, nullptr, &textureBuffer);
	CheckHR(result);

	D3D11_SHADER_RESOURCE_VIEW_DESC srv;
	srv.Format = DXGI_FORMAT_R32_FLOAT;
	srv.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srv.Texture2D.MostDetailedMip = 0;
	srv.Texture2D.MipLevels = -1;
	// 将贴图提取出到srv
	result = d3dDevice->CreateShaderResourceView(textureBuffer, &srv, &textureResourceView);
	CheckHR(result);

	//深度缓冲
	D3D11_DEPTH_STENCIL_VIEW_DESC dsv;
	dsv.Format = DXGI_FORMAT_D32_FLOAT;
	dsv.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsv.Texture2D.MipSlice = 0;
	dsv.Flags = 0;

	// 创建深度缓冲区
	result = d3dDevice->CreateDepthStencilView(textureBuffer, &dsv, &depthStencilView);
	CheckHR(result);
	SafeRelease(textureBuffer);

	// 更新视口
	viewport.Width = static_cast<float>(SHADOW_WIDTH);
	viewport.Height = static_cast<float>(SHADOW_HEIGHT);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;

	// 创建采样描述 用于第二次渲染
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER; // 按边界重复
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	sampDesc.BorderColor[0] = 1.0f;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	result = d3dDevice->CreateSamplerState(&sampDesc, &samplerState);
	CheckHR(result);

	// 创建光栅化描述 第一次渲染
	D3D11_RASTERIZER_DESC rasterizerDesc;
	ZeroMemory(&rasterizerDesc, sizeof(rasterizerDesc));
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = D3D11_CULL_BACK; // 正面剔除，但在bias较大时不需要启用
	rasterizerDesc.FrontCounterClockwise = FALSE;
	rasterizerDesc.DepthClipEnable = true;
	result = d3dDevice->CreateRasterizerState(&rasterizerDesc, shadowPCS.GetNullRasterizerState());
	CheckHR(result);
	
	// 创建着色器 第一次渲染
	Shader shadowShader(L"HLSL\\ShadowVertexShader.hlsl", L"HLSL\\ShadowPixelShader.hlsl");
	shadowShader.CreateShaderAndLayout(d3dDevice, shadowPCS.GetNullVertexShader(), 
		shadowPCS.GetNullPixelShader(),	geoSkyCubeInputLayout, shadowPCS.GetNullInputLayout());
}

ShadowMapping::~ShadowMapping()
{
	SafeRelease(samplerState);
	SafeRelease(depthStencilView);
	SafeRelease(textureResourceView);
	SafeRelease(prev_depthStencilView);
	SafeRelease(prev_RenderTargetView);
}

void ShadowMapping::RenderStart(ID3D11DeviceContext* d3dContext)
{
	using namespace DirectX;
	UINT numViewPorts = 1;
	// 缓存视口
	d3dContext->RSGetViewports(&numViewPorts, &prev_viewport);
	d3dContext->OMGetRenderTargets(1, &prev_RenderTargetView, &prev_depthStencilView);
	// 解绑之前着色器资源
	ID3D11ShaderResourceView* nullSRV = nullptr;
	d3dContext->PSSetShaderResources(3, 1, &nullSRV);

	// 设置视口渲染目标
	d3dContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	d3dContext->OMSetRenderTargets(0, nullptr, depthStencilView);	// 丢弃像素
	d3dContext->RSSetViewports(1, &viewport);
	shadowPCS.CombineToPipe(d3dContext);
}

void ShadowMapping::RenderEnd(ID3D11DeviceContext* d3dContext)
{
	// 恢复之前的视口渲染目标
	d3dContext->RSSetViewports(1, &prev_viewport);
	d3dContext->OMSetRenderTargets(1, &prev_RenderTargetView, prev_depthStencilView);
	SafeRelease(prev_RenderTargetView);
	SafeRelease(prev_depthStencilView);
	// 设置阴影贴图
	d3dContext->PSSetShaderResources(3, 1, &textureResourceView);
	d3dContext->PSSetSamplers(1, 1, &samplerState);
}
