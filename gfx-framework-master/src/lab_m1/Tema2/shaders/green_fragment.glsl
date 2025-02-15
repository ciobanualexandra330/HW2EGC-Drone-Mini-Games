#version 330 core

in vec3 fragColor;
in vec3 fragNormal;
in vec3 fragPosition;

uniform vec3 lightPosition = vec3(0.0, 10.0, 0.0);
uniform vec3 lightColor = vec3(1.0, 1.0, 1.0);
uniform vec3 objectColor = vec3(0.11f, 0.47f, 0.17f);
//uniform vec3 objectColor = vec3(0.0f, 1.0f, 0.0f);

uniform float ambientStrength = 0.8;
uniform float specularStrength = 0.3;
uniform float shininess = 16.0;

out vec4 finalColor;

void main()
{
    vec3 ambient = ambientStrength * lightColor;

    vec3 norm = normalize(fragNormal);
    vec3 lightDir = normalize(lightPosition - fragPosition);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 viewDir = normalize(-fragPosition);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = specularStrength * spec * lightColor;

    vec3 result = (ambient + diffuse + specular) * objectColor;
    finalColor = vec4(result, 1.0);
}
