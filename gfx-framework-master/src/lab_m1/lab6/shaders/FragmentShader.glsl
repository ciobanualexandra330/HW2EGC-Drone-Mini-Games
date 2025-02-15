#version 330

// Input
// TODO(student): Get values from vertex shader
in vec3 frag_color;
in vec3 frag_normal;

// Output
layout(location = 0) out vec4 out_color;
//layout(location = 3) out vec4 out_normal;
uniform float time;

void main()
{
    // TODO(student): Write pixel out color
    float red = mix(1, 0, sin(time));
    float green = mix(1, 0, sin(-time));
    float blue = mix(1, 0, cos(time));
    out_color = vec4(frag_color, 1) + vec4(red, green, blue, 0) ;
    //out_normal = vec4(frag_normal, 1);



}
