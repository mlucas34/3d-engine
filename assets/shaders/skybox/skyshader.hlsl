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
    float3 worldPos : TEXCOORD0;
    float3 normal : NORMAL0;
};

cbuffer constant : register(b0)
{
    row_major float4x4 world;
    row_major float4x4 view;
    row_major float4x4 projection;
};

VS_OUTPUT vsmain(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;

    // Store local position
    output.worldPos = input.position.xyz;

    // Create view matrix without translation
    float4x4 rotView = view;
    rotView[3][0] = 0.0f;
    rotView[3][1] = 0.0f;
    rotView[3][2] = 0.0f;

    // Transform
    float4 pos = mul(input.position, rotView);
    output.position = mul(pos, projection);
    
    // Ensure maximum depth (alternative method)
    output.position.z = output.position.w * 0.999999f;
    
    output.normal = normalize(mul(input.normal, (float3x3) world));
    
    return output;
}

TextureCube Skybox : register(t0);
sampler TextureSampler : register(s0);

struct PS_INPUT
{
    float4 position : SV_POSITION;
    float3 worldPos : TEXCOORD3;
    float3 normal : NORMAL0;
};
 
float4 psmain(PS_INPUT input) : SV_TARGET
{
    // Use the world position as the direction vector for 
    float3 sampleDir = normalize(input.worldPos);
    float3 lightDir = normalize(float3(-1, 1, 1));
    
    float4 texColor = Skybox.Sample(TextureSampler, sampleDir);
        
    float ka = 0.7; // ambient lighting
    float3 ia = float3(1.0, 1.0, 1.0);
    ia *= texColor.rgb;

    float3 ambientLight = ka * ia;
	
    float kd = 1.0f; // diffuse lighting
    float diffuseAmount = max(0.0, dot(lightDir.xyz, input.normal));
    float3 id = float3(1, 1, 1);
    id *= (texColor.rgb);
    float3 lightDiffuse = (kd * diffuseAmount * id);
    
    //float lightDist = length(lightPos.xyz - input.worldPos.xyz);
    
    //float fade = max(0, lightDist - m_lightRadius);
    
    //float constantFunc = 1.0;
    //float linearFunc = 1.0;
    //float quadraticFunc = 1.0;
    
    //float attenuation = constantFunc + linearFunc * fade + quadraticFunc * fade * fade;

    
    // specular light
    //float ks = 0.0f;
    //float3 dirToCam = normalize(input.worldPos.xyz - m_camPos.xyz);
    
    //float3 is = float3(1.0f, 1.0f, 1.0f);
    //float3 reflectedLight = reflect(m_lightDir.xyz, input.normal);
    //float shininess = 10.0f;
    //float specularAmount = pow(max(0.0f, dot(reflectedLight, dirToCam)), shininess);
    
    //float3 specularLight = (ks * specularAmount * is) / attenuation;
	
    // final light using phong lighting equation 
    float3 finalLighting = ambientLight + lightDiffuse; //+ specularLight;

    return texColor * float4(finalLighting, 1.0f);
}

