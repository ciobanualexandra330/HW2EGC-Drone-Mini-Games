#version 330 core

in float noiseValue;
out vec4 FragColor;

uniform bool useFixedColor;
uniform vec3 fixedColor; 

void main() {
    vec3 color1 = vec3(1, 1, 1);
    vec3 color2 = vec3(0.5, 0.5, 0.5);
    vec3 finalColor = mix(color1, color2, noiseValue);
    FragColor = vec4(finalColor, 1.0);
}
