#version 460 core

out vec4 FragColor;

in vec3 fColor;
in vec3 fNormal;
in vec3 viewDir;

void main()
{
    FragColor = vec4(fColor, 1.0);

    //Basic phong shading
    vec3 lightDir = normalize(vec3(1.0, 1.0, 1.0));
    float diff = max(dot(fNormal, lightDir), 0.0);
    vec3 diffuse = diff * vec3(1.0, 1.0, 1.0);
    FragColor = vec4(diffuse, 1.0);

    //Ambient light
    vec3 ambient = vec3(0.1, 0.1, 0.1);
    FragColor = vec4(ambient + diffuse, 1.0);
}