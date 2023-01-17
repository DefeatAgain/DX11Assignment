#pragma once
#include "Utlity.h"
/*
	编译、创建Shader类
*/
class Shader
{
public:
	Shader(const wchar_t* vertexPath, const wchar_t* pixelPath);
	~Shader();

	template<size_t N>
	void CreateShaderAndLayout(ID3D11Device* d3dDevice, ID3D11VertexShader** vertexShader,
		ID3D11PixelShader** pixelShader, const D3D11_INPUT_ELEMENT_DESC(&pInputElementDescs)[N],
		ID3D11InputLayout** inputLayout);

	template<size_t N>
	void CreateShaderAndLayout(ID3D11Device* d3dDevice,
		const D3D11_INPUT_ELEMENT_DESC(&pInputElementDescs)[N]);

	ID3D11VertexShader* vs = nullptr;
	ID3D11PixelShader* ps = nullptr;
	ID3D11InputLayout* il = nullptr;
private:
	ID3DBlob* vsBuffer = nullptr;
	ID3DBlob* psBuffer = nullptr;

	bool CompileD3DShader(const wchar_t* filePath, const char* entry,
		const char* shaderModel, ID3DBlob** buffer);
};

template<size_t N>
inline void Shader::CreateShaderAndLayout(ID3D11Device* d3dDevice, 
	ID3D11VertexShader** vertexShader, ID3D11PixelShader** pixelShader,
	const D3D11_INPUT_ELEMENT_DESC(&pInputElementDescs)[N], ID3D11InputLayout** inputLayout)
{
	HRESULT hr;
	hr = d3dDevice->CreateVertexShader(vsBuffer->GetBufferPointer(), vsBuffer->GetBufferSize(),
		nullptr, vertexShader);
	CheckHR(hr);
	//设置顶点布局
	hr = d3dDevice->CreateInputLayout(pInputElementDescs, ARRAYSIZE(pInputElementDescs),
		vsBuffer->GetBufferPointer(), vsBuffer->GetBufferSize(),
		inputLayout);
	CheckHR(hr);
	hr = d3dDevice->CreatePixelShader(psBuffer->GetBufferPointer(), psBuffer->GetBufferSize(),
		nullptr, pixelShader);
	CheckHR(hr);
}

template<size_t N>
inline void Shader::CreateShaderAndLayout(ID3D11Device* d3dDevice, 
	const D3D11_INPUT_ELEMENT_DESC(&pInputElementDescs)[N])
{
	HRESULT hr;
	hr = d3dDevice->CreateVertexShader(vsBuffer->GetBufferPointer(), vsBuffer->GetBufferSize(),
		nullptr, &vs);
	CheckHR(hr);
	//设置顶点布局
	hr = d3dDevice->CreateInputLayout(pInputElementDescs, ARRAYSIZE(pInputElementDescs),
		vsBuffer->GetBufferPointer(), vsBuffer->GetBufferSize(), &il);
	CheckHR(hr);
	hr = d3dDevice->CreatePixelShader(psBuffer->GetBufferPointer(), psBuffer->GetBufferSize(),
		nullptr, &ps);
	CheckHR(hr);
}
