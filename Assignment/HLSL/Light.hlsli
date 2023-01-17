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
    float4 Specular; // w = 高光范围
};

void ComputeParalleLight(Material mtl, ParallelLight pl, float3 normal, float3 eyePos, float3 objPos,
        out float4 ambient, out float4 diffuse, out float4 specular)
{
    ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
    diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
    specular = float4(0.0f, 0.0f, 0.0f, 0.0f);
    // 环境光
    ambient = mtl.Ambient * pl.Ambient;
    
    float3 lightDir = normalize(pl.Direction.xyz);
    float3 eyeDir = normalize(eyePos - objPos);
    // 漫反射光
    diffuse = mtl.Diffuse * pl.Diffuse * max(0, dot(normal, lightDir));

    float3 halfVec = normalize(lightDir + eyeDir);// / length(lightDir + eyeDir);
    // 高光
    specular = mtl.Specular * pl.Specular * pow(max(0, dot(halfVec, lightDir)), mtl.Specular.w);
}

void ComputeParalleLight(float4 amb, float4 diff, float4 spec, ParallelLight pl, float3 normal, 
    float3 eyePos, float3 objPos, out float4 ambient, out float4 diffuse, out float4 specular)
{
    ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
    diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
    specular = float4(0.0f, 0.0f, 0.0f, 0.0f);
    // 环境光
    ambient = amb * pl.Ambient;
    
    float3 lightDir = normalize(pl.Direction.xyz);
    float3 eyeDir = normalize(eyePos - objPos);
    // 漫反射光
    diffuse = diff * pl.Diffuse * max(0, dot(normal, lightDir));

    float3 halfVec = normalize(lightDir + eyeDir); // / length(lightDir + eyeDir);
    // 高光
    specular = spec * pl.Specular * pow(max(0, dot(halfVec, lightDir)), spec.w * 128.0f);
}


void ComputeSpotLight(Material mtl, SpotLight sl, float3 normal, float3 eyePos, float3 objPos,
        out float4 ambient, out float4 diffuse, out float4 specular)
{
    ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
    diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
    specular = float4(0.0f, 0.0f, 0.0f, 0.0f);
    // 环境光
    ambient = mtl.Ambient * sl.Ambient;
    
    float dist = length(sl.LightPos.xyz - objPos);
    
    if (dist > sl.LightPos.w)
        return;
    
    float3 lightDir = normalize(sl.LightPos.xyz - objPos);
    float3 eyeDir = normalize(eyePos - objPos);
    // 漫反射光
    diffuse = mtl.Diffuse * sl.Diffuse * max(0, dot(normal, lightDir));

    float3 halfVec = (lightDir + eyeDir) / length(lightDir + eyeDir);
    // 高光
    specular = mtl.Specular * sl.Specular * pow(max(0, dot(halfVec, lightDir)), mtl.Specular.w);
    
    // 汇聚因子
    float spot = pow(max(0.0f, dot(-lightDir, sl.Direction.xyz)), sl.Direction.w);
    // 衰减系数
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
    // 环境光
    ambient = amb * sl.Ambient;
    
    float dist = length(sl.LightPos.xyz - objPos);
    
    if (dist > sl.LightPos.w)
        return;
    
    float3 lightDir = normalize(sl.LightPos.xyz - objPos);
    float3 eyeDir = normalize(eyePos - objPos);
    // 漫反射光
    diffuse = diff * sl.Diffuse * max(0, dot(normal, lightDir));

    float3 halfVec = (lightDir + eyeDir) / length(lightDir + eyeDir);
    // 高光
    specular = spec * sl.Specular * pow(max(0, dot(halfVec, lightDir)), spec.w * 128.0f);
    
    // 汇聚因子
    float spot = pow(max(0.0f, dot(-lightDir, sl.Direction.xyz)), sl.Direction.w);
    // 衰减系数
    float att = spot / (sl.att.x + sl.att.y * dist + sl.att.z * dist * dist);

    ambient *= spot;
    diffuse *= att;
    specular *= att;
}