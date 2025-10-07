struct VS_INPUT
{
    float4 position : POSITION0;
    float2 texcoord : TEXCOORD0;
    float3 normal : NORMAL0;
};

struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD0;
    float3 normal : NORMAL1;
    float3 worldPos : TEXCOORD1;
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

    // Transform position to world space
    output.position = mul(input.position, m_world);
    output.worldPos = output.position.xyz;

    // Transform position to screen space
    output.position = mul(output.position, m_view);
    output.position = mul(output.position, m_projection);

    // IMPORTANT: Transform normal to world space
    output.normal = normalize(mul(input.normal, (float3x3) m_world));

    output.texcoord = input.texcoord;
    
    return output;
}