#version 410 core

out vec4 COLOR;

//Texture
//uniform sampler2D albedoMap;

in vec3 FRAGMENT_POSITION;
in vec3 FRAGMENT_COLOR;
in vec3 FRAGMENT_NORMAL;
in vec2 FRAGMENT_TEXCOORD;
in vec3 FRAGMENT_VIEW_DIR;

float rand(vec2 co)
{
    return fract(sin(dot(co.xy, vec2(12.9898, 78.233))) * 43758.5453);
}

float fresnel(vec3 viewDir, vec3 normal, float F0)
{
    // Ensure inputs are normalized
    viewDir = normalize(viewDir);
    normal = normalize(normal);

    // Clamp the dot product to avoid numerical issues
    float cosTheta = clamp(dot(viewDir, normal), 0.0, 1.0);

    // Schlick's approximation for Fresnel
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

void main()
{
    vec3 albedo = vec3(1.0,0.7,0.15);
    float roughness = 0.0;
    float metallic = 0.0;

    //Do basic lighting
    vec3 normal = normalize(FRAGMENT_NORMAL);
    vec3 viewDir = normalize(FRAGMENT_VIEW_DIR);
    vec3 lightDir = normalize(vec3(1.0, 3.0, 1.0));
    vec3 halfDir = normalize(lightDir + viewDir);

    //Calculate the diffuse term
    float NdotL = max(dot(normal, lightDir), 0.0);
    float diffuse = NdotL;

    //Calculate the ambient term
    float ambient = 0.05;

    //Calculate the final color
    vec3 color = albedo * (diffuse) + ambient;

    COLOR = vec4(vec3(color), 1.0);
}