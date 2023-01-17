#include "FromObj.h"
#include "Shader.h"

FromObj::FromObj(ID3D11Device* d3dDevice, ID3D11DeviceContext* d3dContext, const PipeContextSet& pcs)
	:Object(pcs)
{
	// Tower
	using namespace DirectX;

	Shader skyShader(L"HLSL\\ObjVertexShader.hlsl", L"HLSL\\ObjPixelShader.hlsl");
	skyShader.CreateShaderAndLayout(d3dDevice, objInputLayout);
	contextSet.SetShaderAndLayout(skyShader.vs, skyShader.ps, skyShader.il);

	ObjLoader loader1("objs\\tower\\wooden watch tower2.obj");

	std::vector<ObjLoader::Mesh> meshes = std::move(loader1.GetMeshes());
	for (size_t i = 0; i < meshes.size(); i++)
	{
		objects.emplace_back(LoadedObject(meshes[i]));
		std::vector<ID3D11ShaderResourceView*> textures_(3, nullptr);
		for (size_t j = 0; j < meshes[i].textures.size(); j++)
		{
			WCHAR strbuffer[300];
			ConvertToWChar(meshes[i].textures[j].path.c_str(), strbuffer);
			CreateDDSTextureFromFile(d3dDevice, d3dContext, strbuffer, nullptr,	
				&textures_[meshes[i].textures[j].type]);
		}
		objects.back().SetTextures(textures_);
		objects.back().SetWorldMatrix(XMMatrixScaling(3.0f, 3.0f, 3.0f) *
			XMMatrixTranslation(0.0f, -4.0f, 80.0f));
	}
	// Tower2
	ObjLoader loader2("objs\\Medieval tower\\Medieval tower_mid.obj");
	meshes = std::move(loader2.GetMeshes());
	for (size_t i = 0; i < meshes.size(); i++)
	{
		objects.emplace_back(LoadedObject(meshes[i]));
		std::vector<ID3D11ShaderResourceView*> textures_(3, nullptr);
		for (size_t j = 0; j < meshes[i].textures.size(); j++)
		{
			WCHAR strbuffer[300];
			ConvertToWChar(meshes[i].textures[j].path.c_str(), strbuffer);
			CreateDDSTextureFromFile(d3dDevice, d3dContext, strbuffer, nullptr,
				&textures_[meshes[i].textures[j].type], DDS_ALPHA_MODE_STRAIGHT);
		}
		objects.back().SetTextures(textures_);
		objects.back().SetWorldMatrix(XMMatrixScaling(1.5f, 1.5f, 1.5f) *
			XMMatrixTranslation(40.0f, -4.0f, 30.0f));
	}
	objects.push_back(objects.back());
	objects.back().SetWorldMatrix(XMMatrixScaling(1.5f, 1.5f, 1.5f) *
		XMMatrixTranslation(-40.0f, -4.0f, 30.0f));

	// Tree
	ObjLoader loader3("objs\\Tree\\Tree.obj");
	meshes = std::move(loader3.GetMeshes());
	for (size_t i = 0; i < meshes.size(); i++)
	{
		objects.emplace_back(LoadedObject(meshes[i]));
		std::vector<ID3D11ShaderResourceView*> textures_(3, nullptr);
		for (size_t j = 0; j < meshes[i].textures.size(); j++)
		{
			WCHAR strbuffer[300];
			ConvertToWChar(meshes[i].textures[j].path.c_str(), strbuffer);
			CreateDDSTextureFromFile(d3dDevice, d3dContext, strbuffer, nullptr,
				&textures_[meshes[i].textures[j].type], DDS_ALPHA_MODE_STRAIGHT);
		}
		objects.back().SetTextures(textures_);
		objects.back().SetWorldMatrix(XMMatrixScaling(3.0f, 3.0f, 3.0f) *
			XMMatrixTranslation(-20.0f, -1.5f, 50.0f));
	}
}
