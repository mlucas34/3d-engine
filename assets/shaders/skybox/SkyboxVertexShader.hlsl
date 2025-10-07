struct VS_INPUT
{
    float4 position : POSITION0;
};

struct VS_OUTPUT
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

VS_OUTPUT vsmain(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;

    // Store local position
    output.worldPos = input.position.xyz;

    // Create view matrix without translation
    float4x4 rotView = m_view;
    rotView[3][0] = 0.0f;
    rotView[3][1] = 0.0f;
    rotView[3][2] = 0.0f;

    // Transform
    float4 pos = mul(input.position, rotView);
    output.position = mul(pos, m_projection);
    
    // Ensure maximum depth (alternative method)
    output.position.z = output.position.w * 0.999999f;
    
    return output;
}