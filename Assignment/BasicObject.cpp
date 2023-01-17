#include "BasicObject.h"
#include "ConstantStruct.h"

BasicObject::~BasicObject()
{
    SafeRelease(vertexBuffer);
    SafeRelease(indexBuffer);
    ::for_each(textures.begin(), textures.end(), [](auto p) {SafeRelease(p); });
}

BasicObject::BasicObject(const BasicObject& obj) : textures(obj.textures)
{
    if (obj.vertexBuffer) obj.vertexBuffer->AddRef();
    if (obj.indexBuffer) obj.indexBuffer->AddRef();
    ::for_each(textures.begin(), textures.end(), [](auto p) {if (p) p->AddRef(); });
    vertexBuffer = obj.vertexBuffer;
    indexBuffer = obj.indexBuffer;
}

BasicObject& BasicObject::operator=(const BasicObject& obj)
{
    if (this == &obj)
        return *this;
    this->~BasicObject();
    vertexBuffer = obj.vertexBuffer;
    indexBuffer = obj.indexBuffer;
    textures = obj.textures;
    return *this;
}

void GeometryObject::SetBuffer(ID3D11Device* d3dDevice)
{
    HRESULT result;
    // 设置顶点缓冲区描述
    vertexStride = sizeof(Geometry::VertexData);
    D3D11_BUFFER_DESC vbd;
    ZeroMemory(&vbd, sizeof(vbd));
    vbd.Usage = D3D11_USAGE_IMMUTABLE;
    vbd.ByteWidth = geometrydata.vertexData.size() * vertexStride;
    vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vbd.CPUAccessFlags = 0;
    // 新建顶点缓冲区
    D3D11_SUBRESOURCE_DATA subData;
    ZeroMemory(&subData, sizeof(subData));
    subData.pSysMem = geometrydata.vertexData.data();
    result = d3dDevice->CreateBuffer(&vbd, &subData, &vertexBuffer);
    CheckHR(result);

    // 设置索引缓冲区描述
    indexCount = geometrydata.indexData.size();
    D3D11_BUFFER_DESC ibd;
    ZeroMemory(&ibd, sizeof(ibd));
    ibd.Usage = D3D11_USAGE_IMMUTABLE;
    ibd.ByteWidth = indexCount * sizeof(unsigned);
    ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    ibd.CPUAccessFlags = 0;
    // 新建索引缓冲区
    subData.pSysMem = geometrydata.indexData.data();
    result = d3dDevice->CreateBuffer(&ibd, &subData, &indexBuffer);
    CheckHR(result);
}

void GeometryObject::Render(ID3D11DeviceContext* d3dContext)
{
    using namespace DirectX;
    // 设置buffer
    unsigned strides = vertexStride;
    unsigned offsets = 0;
    d3dContext->IASetVertexBuffers(0, 1, &vertexBuffer, &strides, &offsets);
    d3dContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
    // 设置world
    CBWorld cbWorld;
    cbWorld.world = XMMatrixTranspose(world);
    cbWorld.worldInvT = XMMatrixInverse(nullptr, world);
    // 设置材质
    CBMaterial cbMaterial;
    cbMaterial.material = this->material;
    // 更新常量缓冲区
    ID3D11Buffer* cBuffer = nullptr;
    d3dContext->VSGetConstantBuffers(CBWorld_Slot, 1, &cBuffer);
    CopyConstantData(d3dContext, cBuffer, &cbWorld);
    SafeRelease(cBuffer);
    d3dContext->PSGetConstantBuffers(CBMaterial_Slot, 1, &cBuffer);
    CopyConstantData(d3dContext, cBuffer, &cbMaterial);
    SafeRelease(cBuffer);
    // 设置贴图
    for (unsigned i = 0; i < textures.size(); i++)
    {
        d3dContext->PSSetShaderResources(i, 1, &textures[i]);
    }
    // 开始绘制
    d3dContext->DrawIndexed(indexCount, 0, 0);
}

void LoadedObject::SetBuffer(ID3D11Device* d3dDevice)
{
    HRESULT result;
    // 设置顶点缓冲区描述
    vertexStride = sizeof(ObjLoader::VertexData);
    D3D11_BUFFER_DESC vbd;
    ZeroMemory(&vbd, sizeof(vbd));
    vbd.Usage = D3D11_USAGE_IMMUTABLE;
    vbd.ByteWidth = objData.vertexData.size() * vertexStride;
    vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vbd.CPUAccessFlags = 0;
    // 新建顶点缓冲区
    D3D11_SUBRESOURCE_DATA subData;
    ZeroMemory(&subData, sizeof(subData));
    subData.pSysMem = objData.vertexData.data();
    result = d3dDevice->CreateBuffer(&vbd, &subData, &vertexBuffer);
    CheckHR(result);

    // 设置索引缓冲区描述
    indexCount = objData.indexData.size();
    D3D11_BUFFER_DESC ibd;
    ZeroMemory(&ibd, sizeof(ibd));
    ibd.Usage = D3D11_USAGE_IMMUTABLE;
    ibd.ByteWidth = indexCount * sizeof(unsigned);
    ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    ibd.CPUAccessFlags = 0;
    // 新建索引缓冲区
    subData.pSysMem = objData.indexData.data();
    result = d3dDevice->CreateBuffer(&ibd, &subData, &indexBuffer);
    CheckHR(result);
}

void LoadedObject::Render(ID3D11DeviceContext* d3dContext)
{
    using namespace DirectX;
    // 设置buffer
    unsigned strides = vertexStride;
    unsigned offsets = 0;
    d3dContext->IASetVertexBuffers(0, 1, &vertexBuffer, &strides, &offsets);
    d3dContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
    // 设置world
    CBWorld cbWorld;
    cbWorld.world = XMMatrixTranspose(world);
    cbWorld.worldInvT = XMMatrixInverse(nullptr, world);
    // 更新常量缓冲区
    ID3D11Buffer* cBuffer = nullptr;
    d3dContext->VSGetConstantBuffers(CBWorld_Slot, 1, &cBuffer);
    CopyConstantData(d3dContext, cBuffer, &cbWorld);
    SafeRelease(cBuffer);
    // 设置贴图
    for (unsigned i = 0; i < textures.size(); i++)
    {
        d3dContext->PSSetShaderResources(i, 1, &textures[i]);
    }
    // 开始绘制
    d3dContext->DrawIndexed(indexCount, 0, 0);
}
