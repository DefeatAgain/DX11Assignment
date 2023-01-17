#include "Shader.h"
#include<d3dcompiler.h>

Shader::Shader(const wchar_t* vertexPath, const wchar_t* pixelPath)
{
	bool compileResult = CompileD3DShader(vertexPath, "main", "vs_5_0", &vsBuffer);
	if (compileResult == false)
		MessageBox(0, L"Error loading vertex shader!", L"Compile Error", MB_OK);
	compileResult = CompileD3DShader(pixelPath, "main", "ps_5_0", &psBuffer);
	if (compileResult == false)
		MessageBox(0, L"Error loading pixel shader!", L"Compile Error", MB_OK);
}

Shader::~Shader()
{
	SafeRelease(vsBuffer);
	SafeRelease(psBuffer);
	SafeRelease(vs);
	SafeRelease(ps);
	SafeRelease(il);
}

bool Shader::CompileD3DShader(const wchar_t* filePath, const char* entry, const char* shaderModel, ID3DBlob** buffer)
{
	DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;

#if defined( DEBUG ) || defined( _DEBUG )
	shaderFlags |= D3DCOMPILE_DEBUG;
	shaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	ID3DBlob* errorBuffer = nullptr;
	HRESULT result;
	result = D3DCompileFromFile(filePath, 0, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		entry, shaderModel, shaderFlags, 0, buffer, &errorBuffer);

	if (FAILED(result))
	{
		if (errorBuffer != nullptr)
		{
			OutputDebugStringA((char*)errorBuffer->GetBufferPointer());
			errorBuffer->Release();
		}
		return false;
	}

	if (errorBuffer != nullptr)
		errorBuffer->Release();
	return true;
}
