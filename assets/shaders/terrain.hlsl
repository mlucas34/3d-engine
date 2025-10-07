struct VS_INPUT
{
    float4 position : POSITION0;
    float2 texcoord : TEXCOORD0;
    float3 normal : NORMAL0;
    float3 tangent : TANGENT0;
    float3 binormal : BINORMAL0;
};

struct VPS_INOUT
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD0;
    float3 normal : NORMAL0;
    float3 worldPosition : TEXCOORD1;
};

struct LightData
{
    float4 color;
    float4 direction;
};

struct TerrainData
{
    float4 size;
    float heightMapSize;
};

cbuffer constant : register(b0)
{
    row_major float4x4 world;
    row_major float4x4 view;
    row_major float4x4 projection;
    float4 cameraPosition;
    LightData light;
    TerrainData terrain;
};

Texture2D HeightMap : register(t0);
sampler HeightMapSampler : register(s0);

Texture2D GroundMap : register(t1);
sampler GroundMapSampler : register(s1);

Texture2D RockMap : register(t2);
sampler RockMapSampler : register(s2);

VPS_INOUT vsmain(VS_INPUT input)
{
    VPS_INOUT output = (VPS_INOUT) 0;
    float texelSize = 1.0 / terrain.heightMapSize;
    
    float height = HeightMap.SampleLevel(HeightMapSampler, input.texcoord, 0).r;
    output.position = mul(float4(input.position.x * terrain.size.x, 
                                height * terrain.size.y, 
                                input.position.z * terrain.size.z, 1), world);
    
    // world space position
    output.worldPosition = output.position.xyz;
    
    // project to screen
    output.position = mul(output.position, view);
    output.position = mul(output.position, projection);
    output.texcoord = input.texcoord;
    
    float t = HeightMap.SampleLevel(HeightMapSampler, float2(input.texcoord.x, input.texcoord.y - texelSize), 0).r;
    float b = HeightMap.SampleLevel(HeightMapSampler, float2(input.texcoord.x, input.texcoord.y + texelSize), 0).r;
    float l = HeightMap.SampleLevel(HeightMapSampler, float2(input.texcoord.x - texelSize, input.texcoord.y), 0).r;
    float r = HeightMap.SampleLevel(HeightMapSampler, float2(input.texcoord.x + texelSize, input.texcoord.y), 0).r;

    t *= terrain.size.y;
    b *= terrain.size.y;
    l *= terrain.size.y;
    r *= terrain.size.y;

    float3 normal = float3((r - l) * 0.5, 1, (b - t) * 0.5);
    // transform normal to world space (ignore translation)
    float3x3 normalMatrix = (float3x3) world;
    output.normal = normalize(mul(normal, normalMatrix));

    return output;
};

float4 psmain(VPS_INOUT input) : SV_TARGET
{
    // Use the world position as the direction vector for 
    float4 texColor = float4(1, 1, 1, 1);

    float4 ground = GroundMap.Sample(GroundMapSampler, input.texcoord * 100.0f);
    float4 rock = RockMap.Sample(RockMapSampler, input.texcoord * 60.0);
    
    texColor = ground;
    
    float angle = abs(dot(input.normal, float3(0, 1, 0)));
    
    float minAngle = 0.7;
    float maxAngle = 1.0;
    
    if (angle >= minAngle && angle <= maxAngle)
    {
        texColor = lerp(rock, ground, (angle - minAngle) * 1.0 / (maxAngle - minAngle));
    }
    
    if (angle < minAngle)
    {
        texColor = ground;
    }
    
    
    float3 lightDir = normalize(light.direction.xyz);
    float3 directionToCamera = normalize(input.worldPosition - cameraPosition.xyz);
        
    float ka = 0.7; // ambient lighting
    float3 ia = float3(0.09, 0.09, 0.09);
    ia *= texColor.rgb;

    float3 ambientLight = ka * ia;
	
    float kd = 1.0f; // diffuse lighting
    float diffuseAmount = max(0.0, dot(lightDir.xyz, input.normal));
    float3 id = light.color.rgb;
    id *= (texColor.rgb);
    float3 lightDiffuse = (kd * diffuseAmount * id);
    
    //float lightDist = length(lightPos.xyz - input.worldPos.xyz);
    
    //float fade = max(0, lightDist - m_lightRadius);
    
    //float constantFunc = 1.0;
    //float linearFunc = 1.0;
    //float quadraticFunc = 1.0;
    
    //float attenuation = constantFunc + linearFunc * fade + quadraticFunc * fade * fade;

    
    // specular light
    float ks = 0.0f;
    float3 is = float3(1.0f, 1.0f, 1.0f);
    float3 reflectedLight = reflect(lightDir.xyz, input.normal);
    float shininess = 10.0f;
    float specularAmount = pow(max(0.0f, dot(reflectedLight, directionToCamera)), shininess);
    
    float3 specularLight = (ks * specularAmount * is); // / attenuation;
	
    // final light using phong lighting equation 
    float3 finalLighting = ambientLight + lightDiffuse + specularLight;

    return texColor * float4(finalLighting, 1.0f);
}

