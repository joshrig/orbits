#version 410

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec2 vertex_uv;

out vec2 uv;

uniform mat4 mvp;

void main()
{
    gl_Position = mvp * vec4(vertex_position, 1.0);

    uv = vertex_uv;
}
