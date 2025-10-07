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

cbuffer constant : register(b0)
{
    row_major float4x4 world;
    row_major float4x4 view;
    row_major float4x4 projection;
    float4 cameraPosition;
    LightData light;
};

VPS_INOUT vsmain(VS_INPUT input)
{
    VPS_INOUT output = (VPS_INOUT) 0;
    
    // world space position
    float4 worldPos = mul(input.position, world);
    output.worldPosition = worldPos.xyz;
    
    // project to screen
    output.position = mul(worldPos, view);
    output.position = mul(output.position, projection);

    output.texcoord = input.texcoord;

    // transform normal to world space (ignore translation)
    float3x3 normalMatrix = (float3x3) world;
    output.normal = normalize(mul(input.normal, normalMatrix));

    return output;
};

Texture2D Texture : register(t0);
sampler TextureSampler : register(s0);

float4 psmain(VPS_INOUT input) : SV_TARGET
{
        // Use the world position as the direction vector for 
    float3 lightDir = normalize(light.direction.xyz);
    float4 texColor = Texture.Sample(TextureSampler, input.texcoord);
    float3 directionToCamera = normalize(input.worldPosition - cameraPosition.xyz);
        
    float ka = 0.7; // ambient lighting
    float3 ia = float3(1.0, 1.0, 1.0);
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
    float ks = 1.0f;    
    float3 is = float3(1.0f, 1.0f, 1.0f);
    float3 reflectedLight = reflect(lightDir.xyz, input.normal);
    float shininess = 10.0f;
    float specularAmount = pow(max(0.0f, dot(reflectedLight, directionToCamera)), shininess);
    
    float3 specularLight = (ks * specularAmount * is); // / attenuation;
	
    // final light using phong lighting equation 
    float3 finalLighting = ambientLight + lightDiffuse + specularLight;

    return texColor * float4(finalLighting, 1.0f);
}

