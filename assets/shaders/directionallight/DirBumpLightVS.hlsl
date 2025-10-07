struct VS_INPUT
{
    float4 position : POSITION0;
    float2 texcoord : TEXCOORD0;
    float3 normal : NORMAL0;
    float3 tangent : TANGENT0;
    float3 binormal : BINORMAL0;
};

struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD0;
    float3 dirToCam : TEXCOORD1;
    row_major float3x3 tbn : TEXCOORD2;
};

cbuffer constant : register(b0)
{
    row_major float4x4 m_world;
    row_major float4x4 m_view;
    row_major float4x4 m_projection;
    float4 m_lightDir;
    float4 m_camPos;
    float4 m_lightPos;
    float4 m_lightRadius;
    float m_time;
};

VS_OUTPUT vsmain(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;
    
    // world space
    output.position = mul(input.position, m_world);
    output.dirToCam = normalize(output.position.xyz - m_camPos.xyz);
    
    // view space
    output.position = mul(output.position, m_view);
    
    // screen space
    output.position = mul(output.position, m_projection);
    
    output.texcoord = input.texcoord;
    
    output.tbn[0] = normalize(mul(input.normal, m_world));
    output.tbn[1] = normalize(mul(input.binormal, m_world));
    output.tbn[2] = normalize(mul(input.normal, m_world));
    
    
    return output;
}

