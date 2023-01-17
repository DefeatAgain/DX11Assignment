#include "SkyCube.h"
#include "Shader.h"

SkyCube::SkyCube(ID3D11Device* d3dDevice, ID3D11DeviceContext* d3dContext, float skyRadius)
{
	using namespace DirectX;
	GeometryObject sphere(Geometry::CreateSphere(skyRadius));

	HRESULT hr;
	D3D11_RASTERIZER_DESC rasterizerDesc;
	ZeroMemory(&rasterizerDesc, sizeof(rasterizerDesc));
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = D3D11_CULL_NONE;
	rasterizerDesc.FrontCounterClockwise = false;
	rasterizerDesc.DepthClipEnable = true;
	hr = d3dDevice->CreateRasterizerState(&rasterizerDesc, &rasterizerState);
	CheckHR(hr);

	D3D11_SAMPLER_DESC sampleDesc;
	ZeroMemory(&sampleDesc, sizeof(sampleDesc));
	// �������Թ���
	sampleDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	sampleDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampleDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampleDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampleDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampleDesc.MaxAnisotropy = 8;
	sampleDesc.MinLOD = 0;
	sampleDesc.MaxLOD = D3D11_FLOAT32_MAX;
	hr = d3dDevice->CreateSamplerState(&sampleDesc, &samplerState);
	CheckHR(hr);

	D3D11_DEPTH_STENCIL_DESC depthDesc;
	// ��״̬���ڻ�����պУ���Ϊ���ֵΪ1.0ʱĬ���޷�ͨ����Ȳ��ԣ���պ�ʼ�ձ�����Զƽ��
	depthDesc.DepthEnable = true;
	depthDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL; //
	depthDesc.StencilEnable = false;
	hr = d3dDevice->CreateDepthStencilState(&depthDesc, &depthState);
	CheckHR(hr);

	contextSet.SetRasterizerState(rasterizerState);
	contextSet.SetDepthStencilState(depthState);
	contextSet.SetSamplerState(samplerState);

	Shader skyShader(L"HLSL\\SkyVertexShader.hlsl", L"HLSL\\SkyPixelShader.hlsl");
	skyShader.CreateShaderAndLayout(d3dDevice, geoSkyCubeInputLayout);
	contextSet.SetShaderAndLayout(skyShader.vs, skyShader.ps, skyShader.il);

	std::vector<ID3D11ShaderResourceView*> textures(1, nullptr);
	CreateDDSTextureFromFile(d3dDevice, L"Texture\\textureCube.dds",
		nullptr, &textures[0]);
	sphere.SetTextures(std::move(textures));

	objects.push_back(std::move(sphere));
}

SkyCube::~SkyCube()
{
	SafeRelease(depthState);
	SafeRelease(samplerState);
	SafeRelease(rasterizerState);
}