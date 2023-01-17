#pragma once
#include "Geometry.h"
#include "ObjLoader.h"
#include "Light.h"

__declspec(align(16))
class BasicObject
{
public:
    BasicObject():textures(1,nullptr){}
    virtual ~BasicObject();

    BasicObject(const BasicObject&);
    BasicObject(BasicObject&&) = default;
    BasicObject& operator=(const BasicObject&);
    BasicObject& operator=(BasicObject&&) = default;

    // 设置缓冲区
    virtual void SetBuffer(ID3D11Device* d3dDevice) = 0;
    // 绘制
    virtual void Render(ID3D11DeviceContext* deviceContext) = 0;

    ID3D11Buffer* const *  GetVertexBuffer() const { return &vertexBuffer; }
    ID3D11Buffer* const * GetIndexBuffer() const { return &indexBuffer; }
    unsigned GetIndexCount() const { return indexCount; }
    unsigned GetVertexStride() const { return vertexStride; }

    const std::vector <ID3D11ShaderResourceView*>& GetTextureViews() { return textures; }
    void SetTextures(const std::vector<ID3D11ShaderResourceView*>& textures) { this->textures = textures; }

    void XM_CALLCONV SetWorldMatrix(DirectX::FXMMATRIX world) { this->world = world; }
    const DirectX::XMMATRIX GetWorldMatrix() const { return world; }
protected:
    ID3D11Buffer* vertexBuffer = nullptr;                      // 顶点缓冲区
    ID3D11Buffer* indexBuffer = nullptr;                       // 索引缓冲区
    unsigned vertexStride = 0U;                                // 顶点字节大小
    unsigned indexCount = 0U;                                  // 索引数目
    std::vector<ID3D11ShaderResourceView*> textures;          // 纹理
    DirectX::XMMATRIX world = DirectX::XMMatrixIdentity();
};

// 几何体
class GeometryObject :
    public BasicObject
{
public:
    GeometryObject(const Geometry::GeomtryData& data):geometrydata(data) {}
    ~GeometryObject() override {}

    GeometryObject(GeometryObject&&) = default;
    GeometryObject(const GeometryObject&) = default;

    void SetBuffer(ID3D11Device* d3dDevice) override;

    void Render(ID3D11DeviceContext* deviceContext) override;

    void SetMaterial(const Light::Material& mat) { this->material = mat; }
    const Light::Material GetMaterial() { return this->material; }
private:
    Geometry::GeomtryData geometrydata;
    Light::Material material;
};

// 加载模型
class LoadedObject :
    public BasicObject
{
public:
    LoadedObject(const ObjLoader::Mesh& data):objData(data) {}
    ~LoadedObject() override {}

    LoadedObject(LoadedObject&&) = default;
    LoadedObject(const LoadedObject&) = default;

    void SetBuffer(ID3D11Device* d3dDevice) override;

    void Render(ID3D11DeviceContext* deviceContext) override;
private:
    ObjLoader::Mesh objData;
};
