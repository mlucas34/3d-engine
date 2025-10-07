struct VS_INPUT
{
	float4 position : POSITION0;
	float2 texcoord : TEXCOORD0;
    float3 normal : NORMAL0;
};

struct VS_OUTPUT
{
	float4 position: SV_POSITION;
	float2 texcoord : TEXCOORD0;
    float3 normal : TEXCOORD1;
    float3 dirToCam : TEXCOORD2;
};

cbuffer constant: register(b0)
{
	row_major float4x4 m_world;
	row_major float4x4 m_view;
	row_major float4x4 m_projection;
    float4 m_lightDir;
    float4 m_camPos;
};

VS_OUTPUT vsmain( VS_INPUT input )
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	// output.position = lerp(input.position, input.position1, (sin(m_time/ 1000.0f) + 1.0f) / 2.0f);

	// world space
	output.position = mul(input.position, m_world);
    output.dirToCam = normalize(output.position.xyz - m_camPos.xyz);

	// camera view space
	output.position = mul(output.position, m_view);

	// screen space
	output.position = mul(output.position, m_projection);

	output.texcoord = input.texcoord;
    output.normal = input.normal;
	return output;
}