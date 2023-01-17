struct ParallelLight
{
    float4 Ambient;
    float4 Diffuse;
    float4 Specular; 
    float4 Direction;
};

struct SpotLight
{
    float4 Ambient;
    float4 Diffuse;
    float4 Specular; 
    float4 LightPos; // w = range
    float4 Direction; // w = spot
    float4 att; 
};

struct Material
{
    float4 Ambient;
    float4 Diffuse;
    float4 Specular; // w = �߹ⷶΧ
};

void ComputeParalleLight(Material mtl, ParallelLight pl, float3 normal, float3 eyePos, float3 objPos,
        out float4 ambient, out float4 diffuse, out float4 specular)
{
    ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
    diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
    specular = float4(0.0f, 0.0f, 0.0f, 0.0f);
    // ������
    ambient = mtl.Ambient * pl.Ambient;
    
    float3 lightDir = normalize(pl.Direction.xyz);
    float3 eyeDir = normalize(eyePos - objPos);
    // �������
    diffuse = mtl.Diffuse * pl.Diffuse * max(0, dot(normal, lightDir));

    float3 halfVec = normalize(lightDir + eyeDir);// / length(lightDir + eyeDir);
    // �߹�
    specular = mtl.Specular * pl.Specular * pow(max(0, dot(halfVec, lightDir)), mtl.Specular.w);
}

void ComputeParalleLight(float4 amb, float4 diff, float4 spec, ParallelLight pl, float3 normal, 
    float3 eyePos, float3 objPos, out float4 ambient, out float4 diffuse, out float4 specular)
{
    ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
    diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
    specular = float4(0.0f, 0.0f, 0.0f, 0.0f);
    // ������
    ambient = amb * pl.Ambient;
    
    float3 lightDir = normalize(pl.Direction.xyz);
    float3 eyeDir = normalize(eyePos - objPos);
    // �������
    diffuse = diff * pl.Diffuse * max(0, dot(normal, lightDir));

    float3 halfVec = normalize(lightDir + eyeDir); // / length(lightDir + eyeDir);
    // �߹�
    specular = spec * pl.Specular * pow(max(0, dot(halfVec, lightDir)), spec.w * 128.0f);
}


void ComputeSpotLight(Material mtl, SpotLight sl, float3 normal, float3 eyePos, float3 objPos,
        out float4 ambient, out float4 diffuse, out float4 specular)
{
    ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
    diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
    specular = float4(0.0f, 0.0f, 0.0f, 0.0f);
    // ������
    ambient = mtl.Ambient * sl.Ambient;
    
    float dist = length(sl.LightPos.xyz - objPos);
    
    if (dist > sl.LightPos.w)
        return;
    
    float3 lightDir = normalize(sl.LightPos.xyz - objPos);
    float3 eyeDir = normalize(eyePos - objPos);
    // �������
    diffuse = mtl.Diffuse * sl.Diffuse * max(0, dot(normal, lightDir));

    float3 halfVec = (lightDir + eyeDir) / length(lightDir + eyeDir);
    // �߹�
    specular = mtl.Specular * sl.Specular * pow(max(0, dot(halfVec, lightDir)), mtl.Specular.w);
    
    // �������
    float spot = pow(max(0.0f, dot(-lightDir, sl.Direction.xyz)), sl.Direction.w);
    // ˥��ϵ��
    float att = spot / (sl.att.x + sl.att.y * dist + sl.att.z * dist * dist);

    ambient *= spot;
    diffuse *= att;
    specular *= att;
}

void ComputeSpotLight(float4 amb, float4 diff, float4 spec, SpotLight sl, float3 normal, 
    float3 eyePos, float3 objPos,out float4 ambient, out float4 diffuse, out float4 specular)
{
    ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
    diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
    specular = float4(0.0f, 0.0f, 0.0f, 0.0f);
    // ������
    ambient = amb * sl.Ambient;
    
    float dist = length(sl.LightPos.xyz - objPos);
    
    if (dist > sl.LightPos.w)
        return;
    
    float3 lightDir = normalize(sl.LightPos.xyz - objPos);
    float3 eyeDir = normalize(eyePos - objPos);
    // �������
    diffuse = diff * sl.Diffuse * max(0, dot(normal, lightDir));

    float3 halfVec = (lightDir + eyeDir) / length(lightDir + eyeDir);
    // �߹�
    specular = spec * sl.Specular * pow(max(0, dot(halfVec, lightDir)), spec.w * 128.0f);
    
    // �������
    float spot = pow(max(0.0f, dot(-lightDir, sl.Direction.xyz)), sl.Direction.w);
    // ˥��ϵ��
    float att = spot / (sl.att.x + sl.att.y * dist + sl.att.z * dist * dist);

    ambient *= spot;
    diffuse *= att;
    specular *= att;
}