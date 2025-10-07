Texture2D Texture: register(t0);
sampler TextureSampler: register(s0);

struct PS_INPUT
{
    float4 position: SV_POSITION;
	float2 texcoord: TEXCOORD0;
    float3 normal : TEXCOORD1;
    float3 dirToCam : TEXCOORD2; 

};

cbuffer constant: register(b0)
{
    row_major float4x4 m_world;
	row_major float4x4 m_view;
	row_major float4x4 m_projection;
    float4 m_lightDir;

};
 
 float4 psmain( PS_INPUT input ) : SV_TARGET
 {
	float ka = 0.1; // ambient lighting
	float3 ia = float3(1.0, 1.0, 1.0);

	float3 ambientLight = ka * ia;
	
    float kd = 1.0f; // diffuse lighting
    float3 id = float3(1.0f, 1.0f, 1.0f);
    float diffuseAmount = max(0.0, dot(m_lightDir.xyz, input.normal));
	
    float3 lightDiffuse = kd * diffuseAmount * id;
    
    // specular light
    float ks = 1.0f;
    float3 is = float3(1.0f, 1.0f, 1.0f);
    float3 reflectedLight = reflect(m_lightDir.xyz, input.normal);
    float shininess = 1000.0f;
    float specularAmount = pow(max(0.0f, dot(reflectedLight, input.dirToCam)), shininess);
    
    float3 finalSpecular = ks * specularAmount * is;
	
    // final light using phong lighting equation 
    float3 finalLighting = ambientLight + lightDiffuse + finalSpecular;

    return Texture.Sample(TextureSampler, input.texcoord * 0.5) * float4(finalLighting, 1.0f);
}