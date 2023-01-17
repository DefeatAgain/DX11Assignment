#include "ObjLoader.h"

ObjLoader::ObjLoader(const char* objPath)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(objPath,
        aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_FlipUVs | 
        aiProcess_GenNormals); // 需要辅助计算切向量
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        MessageBox(0, L"Read Obj File Fail!", 0, 0);
        return;
    }
    ProcessScene(scene->mRootNode, scene);
}

void ObjLoader::ProcessScene(aiNode* node, const aiScene* scene)
{
    // 读取全部网格
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        ReadData(mesh, scene);
    }
    // 递归处理全部节点
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        ProcessScene(node->mChildren[i], scene);
    }
}

void ObjLoader::ReadData(aiMesh* mesh, const aiScene* scene)
{
    std::vector<VertexData> vertexDatas;
    std::vector<unsigned> indexDatas;
    std::vector<Texture> objTextures;
    // 添加顶点
    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        VertexData vertex;
        vertex.pos.x = mesh->mVertices[i].x;
        vertex.pos.y = mesh->mVertices[i].y;
        vertex.pos.z = mesh->mVertices[i].z;
        vertex.normal.x = mesh->mNormals[i].x;
        vertex.normal.y = mesh->mNormals[i].y;
        vertex.normal.z = mesh->mNormals[i].z;
        vertex.tangent.x = mesh->mTangents[i].x;
        vertex.tangent.y = mesh->mTangents[i].y;
        vertex.tangent.z = mesh->mTangents[i].z;
        // 只需使用第一个顶点
        if (mesh->mTextureCoords)
        {
            vertex.tex.x = mesh->mTextureCoords[0][i].x;
            vertex.tex.y = mesh->mTextureCoords[0][i].y;
        }
        else
            vertex.tex = { 0.0f, 0.0f };
        
        vertexDatas.push_back(std::move(vertex));
    }

    for (unsigned i = 0; i < mesh->mNumFaces; i++)
        for (size_t j = 0; j < mesh->mFaces[i].mNumIndices; j++)
            indexDatas.emplace_back(mesh->mFaces[i].mIndices[j]);
    // 读取模型的贴图
    if (mesh->mMaterialIndex > 0)
    {
        aiMaterial* ai_material = scene->mMaterials[mesh->mMaterialIndex];
        loadMaterialTextures(ai_material, aiTextureType_DIFFUSE, DIFFUSE, objTextures);
        loadMaterialTextures(ai_material, aiTextureType_SPECULAR, SPECULAR, objTextures);
        loadMaterialTextures(ai_material, aiTextureType_HEIGHT, NORMAL, objTextures);
    }
    meshes.push_back(Mesh{ std::move(vertexDatas), std::move(indexDatas), std::move(objTextures) });
}

void ObjLoader::loadMaterialTextures(aiMaterial* ai_material, aiTextureType type, TextureSlot typeName, std::vector<Texture>& textures)
{
    for (unsigned int i = 0; i < ai_material->GetTextureCount(type); i++)
    {
        aiString str;
        ai_material->GetTexture(type, i, &str);
        Texture texture;
        texture.type = typeName;
        texture.path = str.C_Str();
        textures.push_back(std::move(texture));
    }
}

