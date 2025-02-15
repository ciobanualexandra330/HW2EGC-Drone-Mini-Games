#version 330 core

in float noiseValue;
out vec4 FragColor;

uniform bool useFixedColor;
uniform vec3 fixedColor; 

void main() {
    vec3 color1 = vec3(0.1, 0.8, 0.1);
    vec3 color2 = vec3(0.4, 0.2, 0.1);
    vec3 finalColor = mix(color1, color2, noiseValue);
    FragColor = vec4(finalColor, 1.0);
}
