#include "Plane.h"
#include "Geometry.h"

Plane::Plane(ID3D11Device* d3dDevice, ID3D11DeviceContext* d3dContext, const PipeContextSet& pcs)
	:Object(pcs)
{
	using namespace DirectX;
	GeometryObject plane(Geometry::CreatePlane(1000.0f, 1000.0f, 20.0f, 20.0f));
	plane.SetWorldMatrix(XMMatrixTranslation(0.0f, -1.5f, 0.0f));

	Light::Material material;
	material.ambient = { 0.5f, 0.5f, 0.5f, 0.0f };
	material.diffuse = { 0.5f, 0.5f, 0.5f, 0.0f };
	material.specular = { 0.01f, 0.01f, 0.01f, 1.0f };
	plane.SetMaterial(material);

	std::vector<ID3D11ShaderResourceView*> textures(1, nullptr);
	CreateDDSTextureFromFile(d3dDevice, d3dContext, L"Texture\\plane.dds", nullptr, &textures[0]);
	plane.SetTextures(std::move(textures));

	objects.push_back(std::move(plane));
}
