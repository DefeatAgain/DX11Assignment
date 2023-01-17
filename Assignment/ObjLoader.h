#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/material.h>
#include <assimp/postprocess.h>

#include "Utlity.h"

const D3D11_INPUT_ELEMENT_DESC objInputLayout[] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 36, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};
// 贴图的插槽
enum TextureSlot
{
	DIFFUSE,
	SPECULAR,
	NORMAL
};
/*
	管理加载模型的资源类
*/
class ObjLoader
{
public:
	struct VertexData
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT3 normal;
		DirectX::XMFLOAT3 tangent;
		DirectX::XMFLOAT2 tex;
	};

	struct Texture
	{
		TextureSlot type;
		std::string path;
	};

	struct Mesh
	{
		std::vector<VertexData> vertexData;
		std::vector<unsigned> indexData;
		std::vector<Texture> textures;
	};

public:
	ObjLoader(const char* objPath);
	~ObjLoader() {}
	std::vector<Mesh>& GetMeshes() { return meshes; }
private:
	// 递归遍历scene
	void ProcessScene(aiNode* node, const aiScene* scene);
	// 读取mesh的全部数据
	void ReadData(aiMesh* mesh, const aiScene* scene);
	// 读取贴图文件名
	void loadMaterialTextures(aiMaterial* ai_material, aiTextureType type, TextureSlot typeName,
		std::vector<Texture>& textures);

	std::vector<Mesh> meshes;
};
