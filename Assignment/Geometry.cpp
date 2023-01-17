#include "Geometry.h"

Geometry::GeomtryData Geometry::CreateSphere(float radius, unsigned levels, unsigned slices, 
	const DirectX::XMFLOAT4& color)
{
    using namespace DirectX;
    std::vector<VertexData> vertexData;
    float alpha = 0.0f, theta = 0.0f;
    float a_incr = 180.0f / levels;
    float t_incr = 360.0f / slices;
    // 顶端点
    vertexData.emplace_back(VertexData{ XMFLOAT3(0.0f, radius, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f),
                                color, XMFLOAT2(0.0f, 0.0f) });
    // 生成顶点和圆柱侧表面一样 0~levels
    for (unsigned i = 1; i < levels; ++i)
    {
        alpha += a_incr;
        float a_rad = XMConvertToRadians(alpha);
        for (unsigned j = 0; j <= slices; ++j)
        {
            theta += t_incr;
            float t_rad = XMConvertToRadians(theta);
            float x, y, z;
            // 类似欧拉角的计算方法 
            x = radius * sinf(a_rad) * cosf(t_rad);
            y = radius * cosf(a_rad);
            z = radius * sinf(a_rad) * sinf(t_rad);
            // 局部坐标、法向量和纹理坐标
            XMFLOAT3 normal{ x, y, z };
            XMStoreFloat3(&normal, XMVector3Normalize(XMLoadFloat3(&normal)));

            vertexData.emplace_back(VertexData{ XMFLOAT3(x, y, z), normal,
                        color, XMFLOAT2(t_rad / XM_2PI, a_rad / XM_PI) });
        }
        theta = 0.0f;
    }
    // 底端点
    vertexData.emplace_back(VertexData{ XMFLOAT3(0.0f, -radius, 0.0f), XMFLOAT3(0.0f, -1.0f, 0.0f),
                            color, XMFLOAT2(0.0f, 0.0f) });

    // 放入索引
    std::vector<unsigned> index;
    if (levels > 1)
    {
        for (unsigned i = 1; i <= slices + 1; i++)
        {
            index.emplace_back(0);
            index.emplace_back(i);
            index.emplace_back(i % (slices + 1) + 1);
        }
    }
    for (unsigned i = 0; i < levels - 2; i++)
    {
        for (unsigned j = 0; j <= slices; j++)
        {
            index.emplace_back(i * (slices + 1) + j + 1);
            index.emplace_back((i + 1) * (slices + 1) + j + 1);
            index.emplace_back((i + 1) * (slices + 1) + j + 2);

            index.emplace_back(i * (slices + 1) + j + 1);
            index.emplace_back((i + 1) * (slices + 1) + j + 2);
            index.emplace_back(i * (slices + 1) + j + 1 + 1);
        }
    }
    if (levels > 1)
    {
        unsigned j = vertexData.size() - 1;
        unsigned k = (levels - 1) * slices - 1;
        for (unsigned i = 1; i <= slices + 1; i++)
        {
            index.emplace_back(j);
            index.emplace_back(i % (slices + 1) + k);
            index.emplace_back(i + k - 1);
        }
    }
    return { std::move(vertexData), std::move(index) };
}

Geometry::GeomtryData Geometry::CreateBox(float width, float height, float depth, 
	const DirectX::XMFLOAT4& color)
{
    using namespace DirectX;
    GeomtryData geoData;
    geoData.vertexData.resize(24);

    float w2 = width / 2, h2 = height / 2, d2 = depth / 2;

    // 右面
    geoData.vertexData[0].pos = XMFLOAT3(w2, -h2, -d2);
    geoData.vertexData[1].pos = XMFLOAT3(w2, h2, -d2);
    geoData.vertexData[2].pos = XMFLOAT3(w2, h2, d2);
    geoData.vertexData[3].pos = XMFLOAT3(w2, -h2, d2);
    // 左面
    geoData.vertexData[4].pos = XMFLOAT3(-w2, -h2, d2);
    geoData.vertexData[5].pos = XMFLOAT3(-w2, h2, d2);
    geoData.vertexData[6].pos = XMFLOAT3(-w2, h2, -d2);
    geoData.vertexData[7].pos = XMFLOAT3(-w2, -h2, -d2);
    // 顶面
    geoData.vertexData[8].pos = XMFLOAT3(-w2, h2, -d2);
    geoData.vertexData[9].pos = XMFLOAT3(-w2, h2, d2);
    geoData.vertexData[10].pos = XMFLOAT3(w2, h2, d2);
    geoData.vertexData[11].pos = XMFLOAT3(w2, h2, -d2);
    // 底面
    geoData.vertexData[12].pos = XMFLOAT3(w2, -h2, -d2);
    geoData.vertexData[13].pos = XMFLOAT3(w2, -h2, d2);
    geoData.vertexData[14].pos = XMFLOAT3(-w2, -h2, d2);
    geoData.vertexData[15].pos = XMFLOAT3(-w2, -h2, -d2);
    // 背面
    geoData.vertexData[16].pos = XMFLOAT3(w2, -h2, d2);
    geoData.vertexData[17].pos = XMFLOAT3(w2, h2, d2);
    geoData.vertexData[18].pos = XMFLOAT3(-w2, h2, d2);
    geoData.vertexData[19].pos = XMFLOAT3(-w2, -h2, d2);
    // 正面
    geoData.vertexData[20].pos = XMFLOAT3(-w2, -h2, -d2);
    geoData.vertexData[21].pos = XMFLOAT3(-w2, h2, -d2);
    geoData.vertexData[22].pos = XMFLOAT3(w2, h2, -d2);
    geoData.vertexData[23].pos = XMFLOAT3(w2, -h2, -d2);

    for (unsigned i = 0; i < 4; i++)
    {
        // 右面
        geoData.vertexData[i].normal = XMFLOAT3(1.0f, 0.0f, 0.0f);
        geoData.vertexData[i].color = color;
        // 左面
        geoData.vertexData[i + 4].normal = XMFLOAT3(-1.0f, 0.0f, 0.0f);
        geoData.vertexData[i + 4].color = color;
        // 顶面
        geoData.vertexData[i + 8].normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
        geoData.vertexData[i + 8].color = color;
        // 底面
        geoData.vertexData[i + 12].normal = XMFLOAT3(0.0f, -1.0f, 0.0f);
        geoData.vertexData[i + 12].color = color;
        // 背面
        geoData.vertexData[i + 16].normal = XMFLOAT3(0.0f, 0.0f, 1.0f);
        geoData.vertexData[i + 16].color = color;
        // 正面
        geoData.vertexData[i + 20].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
        geoData.vertexData[i + 20].color = color;
    }
    for (unsigned i = 0; i < 6; i++)
    {
        geoData.vertexData[i * 4].tex = XMFLOAT2(0.0f, 1.0f);
        geoData.vertexData[i * 4 + 1].tex = XMFLOAT2(0.0f, 0.0f);
        geoData.vertexData[i * 4 + 2].tex = XMFLOAT2(1.0f, 0.0f);
        geoData.vertexData[i * 4 + 3].tex = XMFLOAT2(1.0f, 1.0f);
    }
    geoData.indexData = {
        0, 1, 2, 2, 3, 0,        // 右面(+X面)
        4, 5, 6, 6, 7, 4,        // 左面(-X面)
        8, 9, 10, 10, 11, 8,    // 顶面(+Y面)
        12, 13, 14, 14, 15, 12,    // 底面(-Y面)
        16, 17, 18, 18, 19, 16, // 背面(+Z面)
        20, 21, 22, 22, 23, 20    // 正面(-Z面)
    };

    return geoData;
}

Geometry::GeomtryData Geometry::CreateCylinder(float radius, float height, 
	unsigned slices, unsigned level, const DirectX::XMFLOAT4& color)
{
    using namespace DirectX;
    //上下圆面
    std::vector<VertexData> top;
    std::vector<VertexData> bottom;
    float angle1 = 0.0f;
    float incr = 360.0f / slices;
    top.resize(slices + 1);
    bottom.resize(slices + 1);
    float h2 = height / 2.0f;
    top[0] = { XMFLOAT3(0.0f, h2, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f),
                   color, XMFLOAT2(0.5f, 0.5f) };
    for (unsigned i = 1; i <= slices; i++)
    {
        float rad = XMConvertToRadians(angle1);
        float u = cosf(rad) * radius + 0.5f;
        float v = sinf(rad) * radius + 0.5f;
        top[i].pos = XMFLOAT3(radius * cosf(rad), h2, radius * sinf(rad));
        top[i].normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
        top[i].color = color;
        top[i].tex = XMFLOAT2(u, v);
        angle1 += incr;
    }
    bottom[0] = { XMFLOAT3(0.0f, -h2, 0.0f), XMFLOAT3(0.0f, -1.0f, 0.0f),
                   color, XMFLOAT2(0.5f, 0.5f) };
    angle1 = 0.0f;
    for (unsigned i = 1; i <= slices; i++)
    {
        float rad = XMConvertToRadians(angle1);
        float u = cosf(rad) * radius + 0.5f;
        float v = sinf(rad) * radius + 0.5f;
        bottom[i].pos = XMFLOAT3(radius * cosf(rad), -h2, radius * sinf(rad));
        bottom[i].normal = XMFLOAT3(0.0f, -1.0f, 0.0f);
        bottom[i].color = color;
        bottom[i].tex = XMFLOAT2(u, v);
        angle1 += incr;
    }

    //侧面
    float hIncr = height / level;
    std::vector<VertexData> side;
    float y = -h2;
    for (unsigned i = 0; i <= level; i++)
    {
        angle1 = 0.0f;
        for (unsigned j = 0; j <= slices; j++)
        {
            float rad = XMConvertToRadians(angle1);
            float u = rad;
            float v = 1.0f - (float)i / level;
            side.emplace_back(VertexData{ XMFLOAT3(radius * cosf(rad), y, radius * sinf(rad)),
                XMFLOAT3(cosf(rad), 0.0f, sinf(rad)), color, XMFLOAT2(u, v) });
            angle1 += incr;
        }
        y += hIncr;
    }
    std::vector<VertexData> all(top.size() + bottom.size() + side.size());
    std::uninitialized_copy(std::make_move_iterator(top.begin()),
        std::make_move_iterator(top.end()), all.begin());
    std::uninitialized_copy(std::make_move_iterator(bottom.begin()),
        std::make_move_iterator(bottom.end()), all.begin() + top.size());
    std::uninitialized_copy(std::make_move_iterator(side.begin()),
        std::make_move_iterator(side.end()), all.begin() + top.size() + bottom.size());


    std::vector<unsigned> index;
    unsigned i = 1;
    for (unsigned j = 0; j < slices; j++, i++)
    {
        index.emplace_back(0);
        index.emplace_back(i % slices + 1);
        index.emplace_back(i);
    }
    i = 1;
    for (unsigned j = 0; j < slices; j++, i++)
    {
        index.emplace_back(slices + 1);
        index.emplace_back(i + slices + 1);
        index.emplace_back(i % slices + 2 + slices);
    }
    i = 2 + slices * 2;
    for (unsigned j = 0; j <= level; j++)
    {
        for (unsigned k = 0; k < slices; k++)
        {
            index.emplace_back(j * (slices + 1) + k + i);
            index.emplace_back((j + 1) * (slices + 1) + k + i);
            index.emplace_back((j + 1) * (slices + 1) + k + 1 + i);

            index.emplace_back(j * (slices + 1) + k + i);
            index.emplace_back((j + 1) * (slices + 1) + k + 1 + i);
            index.emplace_back(j * (slices + 1) + k + 1 + i);
        }
    }
    return { std::move(all), std::move(index) };
}

Geometry::GeomtryData Geometry::CreatePlane(float width, float depth, float u, float v, 
    const DirectX::XMFLOAT4& color)
{
    using namespace DirectX;

    std::vector<VertexData> vertexData;

    vertexData.emplace_back(VertexData{ XMFLOAT3(-width / 2, 0.0f, -depth / 2), XMFLOAT3(0.0f, 1.0f, 0.0f),
                 color, XMFLOAT2(0.0f, v) });
    vertexData.emplace_back(VertexData{ XMFLOAT3(-width / 2, 0.0f, depth / 2), XMFLOAT3(0.0f, 1.0f, 0.0f),
                 color, XMFLOAT2(0.0f, 0.0f) });
    vertexData.emplace_back(VertexData{ XMFLOAT3(width / 2, 0.0f, depth / 2), XMFLOAT3(0.0f, 1.0f, 0.0f),
                 color, XMFLOAT2(u, 0.0f) });
    vertexData.emplace_back(VertexData{ XMFLOAT3(width / 2, 0.0f, -depth / 2), XMFLOAT3(0.0f, 1.0f, 0.0f),
                 color, XMFLOAT2(u, v) });

    return { std::move(vertexData), { 0, 1, 2, 2, 3, 0 } };
}
