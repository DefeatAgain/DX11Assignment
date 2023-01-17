#pragma once
#include "Utlity.h"
#include "PipeContextSet.h"

namespace
{
	const UINT SHADOW_WIDTH = 1024;
	const UINT SHADOW_HEIGHT = 1024;
}
/*
	������Ӱӳ�����
*/
class ShadowMapping
{
public:
	ShadowMapping(ID3D11Device* d3dDevice);
	~ShadowMapping();
	void RenderStart(ID3D11DeviceContext* d3dContext);
	void RenderEnd(ID3D11DeviceContext* d3dContext);
	ID3D11ShaderResourceView*& GetTexture() { return textureResourceView; }
private:
	PipeContextSet shadowPCS;
	ID3D11DepthStencilView* depthStencilView = nullptr;
	ID3D11ShaderResourceView* textureResourceView = nullptr;
	D3D11_VIEWPORT viewport;
	ID3D11SamplerState* samplerState;
	// ����֮ǰ����ȾĿ��״̬���ӿ�
	D3D11_VIEWPORT prev_viewport;
	ID3D11RenderTargetView* prev_RenderTargetView = nullptr;
	ID3D11DepthStencilView* prev_depthStencilView = nullptr;
};

