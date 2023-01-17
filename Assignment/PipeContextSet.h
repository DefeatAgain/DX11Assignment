#pragma once
#include "Utlity.h"
// 复制新资源释放旧资源
template<typename T> void SetNewResouce(T*& pOriRes, T* pNewRes);
/*
    管道设定类
*/
class PipeContextSet
{
    friend void AddRefCount(const PipeContextSet& pipeSet);
    template<typename T> friend void SetNewResouce(T*& pOriRes, T* pNewRes);
public:
    PipeContextSet() {}
    PipeContextSet(ID3D11Device* d3dDevice);
    ~PipeContextSet();
    PipeContextSet(const PipeContextSet& pipeSet);
    PipeContextSet(PipeContextSet&& pipeSet) = default;
    PipeContextSet& operator=(const PipeContextSet& pipeSet);
    PipeContextSet& operator=(PipeContextSet&& pipeSet) = default;
    // 管道绑定
    void CombineToPipe(ID3D11DeviceContext* d3dContext);
    // 仅可获取指向NULL的资源对象，否则应该使用SET方法
    ID3D11SamplerState** GetNullSamplerState() { IsNull(samplerState); return &samplerState; }
    ID3D11RasterizerState** GetNullRasterizerState() { IsNull(rasterizerState); return &rasterizerState; }
    ID3D11DepthStencilState** GetNullDepthStencilState() { IsNull(depthStencilState); return &depthStencilState; }
    ID3D11BlendState** GetNullBlendState() { IsNull(blendState); return &blendState; }
    ID3D11VertexShader** GetNullVertexShader() { IsNull(vertexShader);  return &vertexShader; }
    ID3D11PixelShader** GetNullPixelShader() { IsNull(pixelShader); return &pixelShader; }
    ID3D11InputLayout** GetNullInputLayout() { IsNull(inputLayout); return &inputLayout; }

    void SetSamplerState(ID3D11SamplerState* ss) 
        { SetNewResouce(samplerState, ss); }

    void SetRasterizerState(ID3D11RasterizerState* rs) 
        { SetNewResouce(rasterizerState, rs); }

    void SetDepthStencilState(ID3D11DepthStencilState* dss) 
        { SetNewResouce(depthStencilState, dss); }

    void SetBlendState(ID3D11BlendState* bs) 
        { SetNewResouce(blendState, bs); }

    void SetShaderAndLayout(ID3D11VertexShader* vs, ID3D11PixelShader* ps,
        ID3D11InputLayout* inputLayout);

    const static DirectX::XMMATRIX GetViewMatrix() { return view; }
    const static DirectX::XMMATRIX GetProjMatrix() { return proj; }
    const static DirectX::XMVECTOR GetEyePos() { return eyePos; }
    static void XM_CALLCONV SetViewMatrix(DirectX::FXMMATRIX v) { view = v; }
    static void XM_CALLCONV SetProjMatrix(DirectX::FXMMATRIX p) { proj = p; }
    static void XM_CALLCONV SetEyePos(DirectX::FXMVECTOR pos) { eyePos = pos; }
    static void CreateConstantBuffer(ID3D11Device* d3dDevice, ID3D11DeviceContext* d3dContext);
    const static std::vector<ID3D11Buffer*>& GetConstantBuffers() { return constantBuffers; }
    static void ReleaseConstantBuffer();
private:
    ID3D11SamplerState* samplerState = nullptr;                // 采样器状态
    ID3D11RasterizerState* rasterizerState = nullptr;          // 光栅化状态
    ID3D11DepthStencilState* depthStencilState = nullptr;      // 深度模板状态
    ID3D11BlendState* blendState = nullptr;                    // 混合状态

    ID3D11VertexShader* vertexShader = nullptr;                // 顶点着色器
    ID3D11PixelShader* pixelShader = nullptr;                  // 像素着色器
    ID3D11InputLayout* inputLayout = nullptr;                  // 顶点布局

    static std::vector<ID3D11Buffer*> constantBuffers;         // 常量缓冲区

    static DirectX::XMMATRIX view;                             // 视图矩阵 
    static DirectX::XMMATRIX proj;                             // 投影矩阵 
    static DirectX::XMVECTOR eyePos;                           // 相机位置

private:
    template<typename T> void IsNull(T* pRes) { if (pRes) 
        throw std::runtime_error("ERROR! This pointer has pointed a resource!"); }
};

template<typename T>
inline void SetNewResouce(T*& pOriRes, T* pNewRes)
{
    if (!pNewRes)
        return;
    pNewRes->AddRef();
    SafeRelease(pOriRes);
    pOriRes = pNewRes;
}
