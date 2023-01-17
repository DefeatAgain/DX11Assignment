#include "PipeContextSet.h"
#include "ConstantStruct.h"

std::vector<ID3D11Buffer*> PipeContextSet::constantBuffers;
DirectX::XMMATRIX PipeContextSet::view = DirectX::XMMatrixIdentity();
DirectX::XMMATRIX PipeContextSet::proj = DirectX::XMMatrixIdentity();
DirectX::XMVECTOR PipeContextSet::eyePos;

PipeContextSet::PipeContextSet(ID3D11Device* d3dDevice)
{
	// 默认采样描述
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	CheckHR(d3dDevice->CreateSamplerState(&sampDesc, &samplerState));

	// 默认光栅化描述
	D3D11_RASTERIZER_DESC rasterizerDesc;
	ZeroMemory(&rasterizerDesc, sizeof(rasterizerDesc));
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = D3D11_CULL_BACK;
	rasterizerDesc.FrontCounterClockwise = FALSE;
	rasterizerDesc.DepthBias = 0;
	rasterizerDesc.SlopeScaledDepthBias = 0.0f;
	rasterizerDesc.DepthBiasClamp = 0.0f;
	rasterizerDesc.DepthClipEnable = TRUE;
	rasterizerDesc.ScissorEnable = FALSE;
	rasterizerDesc.MultisampleEnable = FALSE;
	rasterizerDesc.AntialiasedLineEnable = FALSE;
	CheckHR(d3dDevice->CreateRasterizerState(&rasterizerDesc, &rasterizerState));
}

PipeContextSet::~PipeContextSet()
{
    SafeRelease(samplerState);
    SafeRelease(rasterizerState);
    SafeRelease(depthStencilState);
    SafeRelease(blendState);
	SafeRelease(vertexShader);
	SafeRelease(pixelShader);
	SafeRelease(inputLayout);
}


void PipeContextSet::CombineToPipe(ID3D11DeviceContext* d3dContext)
{
	d3dContext->VSSetShader(vertexShader, nullptr, 0);
	d3dContext->PSSetShader(pixelShader, nullptr, 0);
	d3dContext->IASetInputLayout(inputLayout);
	d3dContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	d3dContext->RSSetState(rasterizerState);
	d3dContext->OMSetDepthStencilState(depthStencilState, 0);
	d3dContext->PSSetSamplers(0, 1, &samplerState);
	//d3dContext->OMSetBlendState(blendState, nullptr, 0xFFFFFFFF);
}

void PipeContextSet::SetShaderAndLayout(ID3D11VertexShader* vs, ID3D11PixelShader* ps,
	ID3D11InputLayout* il)
{
	SetNewResouce(vertexShader, vs);
	SetNewResouce(pixelShader, ps);
	SetNewResouce(inputLayout, il);
}

void PipeContextSet::CreateConstantBuffer(ID3D11Device* d3dDevice, ID3D11DeviceContext* d3dContext)
{
	constantBuffers.resize(6);
	// 设置常量缓冲区描述
	D3D11_BUFFER_DESC cbd;
	ZeroMemory(&cbd, sizeof(cbd));
	cbd.Usage = D3D11_USAGE_DYNAMIC;
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	// 新建用于VS和PS的常量缓冲区
	cbd.ByteWidth = sizeof(CBWorld);
	CheckHR(d3dDevice->CreateBuffer(&cbd, nullptr, &constantBuffers[CBWorld_Slot]));

	cbd.ByteWidth = sizeof(CBViewEyePos);
	CheckHR(d3dDevice->CreateBuffer(&cbd, nullptr, &constantBuffers[CBViewEyePos_Slot]));

	cbd.ByteWidth = sizeof(CBProj);
	CheckHR(d3dDevice->CreateBuffer(&cbd, nullptr, &constantBuffers[CBProj_Slot]));

	cbd.ByteWidth = sizeof(CBLight);
	CheckHR(d3dDevice->CreateBuffer(&cbd, nullptr, &constantBuffers[CBLight_Slot]));

	cbd.ByteWidth = sizeof(CBMaterial);
	CheckHR(d3dDevice->CreateBuffer(&cbd, nullptr, &constantBuffers[CBMaterial_Slot]));

	cbd.ByteWidth = sizeof(CBToLightSpace);
	CheckHR(d3dDevice->CreateBuffer(&cbd, nullptr, &constantBuffers[CBToLightSpace_Slot]));

	d3dContext->VSSetConstantBuffers(CBWorld_Slot, 1, &constantBuffers[CBWorld_Slot]);
	d3dContext->VSSetConstantBuffers(CBViewEyePos_Slot, 1, &constantBuffers[CBViewEyePos_Slot]);
	d3dContext->VSSetConstantBuffers(CBProj_Slot, 1, &constantBuffers[CBProj_Slot]);
	d3dContext->VSSetConstantBuffers(CBToLightSpace_Slot, 1, &constantBuffers[CBToLightSpace_Slot]);
	d3dContext->PSSetConstantBuffers(CBViewEyePos_Slot, 1, &constantBuffers[CBViewEyePos_Slot]);
	d3dContext->PSSetConstantBuffers(CBLight_Slot, 1, &constantBuffers[CBLight_Slot]);
	d3dContext->PSSetConstantBuffers(CBMaterial_Slot, 1, &constantBuffers[CBMaterial_Slot]);
}

void PipeContextSet::ReleaseConstantBuffer()
{
	for_each(constantBuffers.begin(), constantBuffers.end(), [](auto p) {SafeRelease(p); });
}

PipeContextSet::PipeContextSet(const PipeContextSet& pipeSet)
{
	AddRefCount(pipeSet);
	samplerState = pipeSet.samplerState;
	rasterizerState = pipeSet.rasterizerState;
	depthStencilState = pipeSet.depthStencilState;
	blendState = pipeSet.blendState;
	vertexShader = pipeSet.vertexShader;
	pixelShader = pipeSet.pixelShader;
	inputLayout = pipeSet.inputLayout;
}

PipeContextSet& PipeContextSet::operator=(const PipeContextSet& pipeSet)
{
	if (this == &pipeSet)
		return *this;
	this->~PipeContextSet();
	samplerState = pipeSet.samplerState;
	rasterizerState = pipeSet.rasterizerState;
	depthStencilState = pipeSet.depthStencilState;
	blendState = pipeSet.blendState;
	vertexShader = pipeSet.vertexShader;
	pixelShader = pipeSet.pixelShader;
	inputLayout = pipeSet.inputLayout;
	return *this;
}

void AddRefCount(const PipeContextSet& pipeSet)
{
	if (pipeSet.rasterizerState) pipeSet.rasterizerState->AddRef();
	if (pipeSet.depthStencilState) pipeSet.depthStencilState->AddRef();
	if (pipeSet.samplerState) pipeSet.samplerState->AddRef();
	if (pipeSet.blendState) pipeSet.blendState->AddRef();
	if (pipeSet.vertexShader) pipeSet.vertexShader->AddRef();
	if (pipeSet.pixelShader) pipeSet.pixelShader->AddRef();
	if (pipeSet.inputLayout) pipeSet.inputLayout->AddRef();
}
