TextureCube Skybox : register(t0);
sampler TextureSampler : register(s0);

struct PS_INPUT
{
    float4 position : SV_POSITION;
    float3 worldPos : TEXCOORD3;
};

cbuffer constant : register(b0)
{
    row_major float4x4 m_world;
    row_major float4x4 m_view;
    row_major float4x4 m_projection;
    float4 m_lightDir;
    float4 m_camPos;
};
 
float4 psmain(PS_INPUT input) : SV_TARGET
{
    // Use the world position as the direction vector for 
    float3 sampleDir = normalize(input.worldPos);
    return Skybox.Sample(TextureSampler, sampleDir);
}