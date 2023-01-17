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
// ��ͼ�Ĳ��
enum TextureSlot
{
	DIFFUSE,
	SPECULAR,
	NORMAL
};
/*
	�������ģ�͵���Դ��
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
	// �ݹ����scene
	void ProcessScene(aiNode* node, const aiScene* scene);
	// ��ȡmesh��ȫ������
	void ReadData(aiMesh* mesh, const aiScene* scene);
	// ��ȡ��ͼ�ļ���
	void loadMaterialTextures(aiMaterial* ai_material, aiTextureType type, TextureSlot typeName,
		std::vector<Texture>& textures);

	std::vector<Mesh> meshes;
};
