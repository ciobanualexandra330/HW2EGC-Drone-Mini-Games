#version 330

// Input
// TODO(student): Get vertex attributes from each location
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 3) in vec3 v_color;



// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform float time;

// Output
// TODO(student): Output values to fragment shader
out vec3 frag_color;
out vec3 frag_normal;

void main()
{
    // TODO(student): Send output to fragment shader
    frag_color = v_color;
    frag_normal = v_normal;

    // TODO(student): Compute gl_Position
    gl_Position = Projection * View * Model * vec4(v_position.x + sin(time), v_position.y + sin(-time), v_position.z - cos(time), 1.0);

}
