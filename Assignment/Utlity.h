#pragma once
#include<Windows.h>
#include<d3d11.h>
#include<DirectXMath.h>
#include "DirectXTex\DDSTextureLoader11.h"

#include<cassert>
#include<iostream>
#include<vector>
#include<memory>
#include"AlignedAllocator.h"

// �ͷ�IUnKnown��Դ
template<typename T>
inline void SafeRelease(T*& ptr)
{
	if (ptr)
		ptr->Release();
	ptr = nullptr;
}
// ��HR���з���
inline void CheckHResult(HRESULT result, const WCHAR* file = __FILEW__, const DWORD line = __LINE__)
{
	WCHAR strBufferError[256];
	WCHAR strBuffer[3000];
	if (FAILED(result))
	{
		FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			nullptr, result, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			strBufferError, 256, nullptr);
		swprintf_s(strBuffer, 3000, L"�ļ�����%ls\n�кţ�%lu\n����%ls\n", file, line, strBufferError);
		OutputDebugStringW(strBuffer);
		MessageBox(0, strBuffer, 0, 0);
	}
}
#define CheckHR(hr) CheckHResult(hr, __FILEW__, (DWORD)__LINE__)
// ����������Դ
template<typename T>
inline void CopyConstantData(ID3D11DeviceContext* d3dContext, ID3D11Resource* dest, T* constantData)
{
	D3D11_MAPPED_SUBRESOURCE mappedData;
	d3dContext->Map(dest, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData);
	memcpy_s(mappedData.pData, sizeof(T), constantData, sizeof(T));
	d3dContext->Unmap(dest, 0);
}
// ���std::for_each
template<typename beg, typename fn>
inline void for_each(beg start, beg end, fn func)
{
	for (; start != end; start++)
	{
		func(*start);
	}
}
// ���ַ�ת���ַ�
inline void ConvertToWChar(const char* pSrc, wchar_t* pDest)
{
	DWORD ssize = MultiByteToWideChar(CP_ACP, 0, pSrc, -1, nullptr, 0);
	MultiByteToWideChar(CP_ACP, 0, pSrc, -1, pDest, ssize);
}
// ���ַ�ת���ַ�
inline void ConvertToChar(const wchar_t* pSrc, char* pDest)
{
	DWORD ssize = WideCharToMultiByte(CP_OEMCP, 0, pSrc, -1, nullptr, 0, nullptr, FALSE);
	WideCharToMultiByte(CP_OEMCP, 0, pSrc, -1, pDest, ssize, nullptr, FALSE);
}