#version 460 core
in vec2 TexCoords;
out vec4 FragColor;
uniform sampler2D TEXTURE;
uniform bool USE_NORMAL_MAP;
uniform sampler2D NORMAL;
uniform float TIME;
void main()
{
    vec4 color = texture(TEXTURE, TexCoords);
    if(!USE_NORMAL_MAP)
    {
        FragColor = color;
        return;
    }
    else
    {
        vec4 normal = texture(NORMAL, TexCoords);
        vec3 lightDir = normalize(vec3(sin(TIME),cos(TIME), 1.0));
        normal.rgb = normalize(normal.rgb * 2.0 - 1.0);
        float diff = max(dot(normal.rgb, lightDir), 0.0);
        vec3 ambient = vec3(0.2);
        vec3 lightColor = vec3(1.0);
        vec3 result = (ambient + diff) * color.rgb * lightColor;
        FragColor = vec4(result, color.a);
    }
}