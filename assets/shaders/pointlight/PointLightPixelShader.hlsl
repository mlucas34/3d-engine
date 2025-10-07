Texture2D Texture : register(t0);
sampler TextureSampler : register(s0);

struct PS_INPUT
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD0;
    float3 normal : NORMAL0;
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
    float m_lightRadius;
    float m_time;
};
 
float4 psmain(PS_INPUT input) : SV_TARGET
{
    float4 texColor = Texture.Sample(TextureSampler, float2(input.texcoord.x, 1.0 - input.texcoord.y));
    
    float ka = 0.1; // ambient lighting
    float3 ia = float3(1.0, 1.0, 1.0);

    float3 ambientLight = ka * ia;
	
    float kd = 1.0f; // diffuse lighting
    float3 lightDir = normalize(m_lightPos.xyz - input.worldPos.xyz);
    float lightDist = length(m_lightPos.xyz - input.worldPos.xyz);
    
    float fade = max(0, lightDist - m_lightRadius);
    
    float constantFunc = 1.0;
    float linearFunc = 1.0;
    float quadraticFunc = 1.0;
    
    float attenuation = constantFunc + linearFunc * fade + quadraticFunc * fade * fade;
    
    float3 id = float3(1.0f, 1.0f, 1.0f);
    float diffuseAmount = max(0.0, dot(lightDir.xyz, input.normal));
    float3 lightDiffuse = (kd * diffuseAmount * id) / attenuation;
    
    // specular light
    float ks = 1.0f;
    float3 dirToCam = normalize(input.worldPos.xyz - m_camPos.xyz);
    
    float3 is = float3(1.0f, 1.0f, 1.0f);
    float3 reflectedLight = reflect(lightDir.xyz, input.normal);
    float shininess = 10.0f;
    float specularAmount = pow(max(0.0f, dot(reflectedLight, dirToCam)), shininess);
    
    float3 specularLight = (ks * specularAmount * is) / attenuation;
	
    // final light using phong lighting equation 
    float3 finalLighting = ambientLight + lightDiffuse + specularLight;

    return texColor * float4(finalLighting, 1.0f);
}