#version 410

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec2 vertex_uv;
layout(location = 2) in vec3 vertex_normal;

out vec2 uv;
out vec3 normal;

uniform mat4 mvp;

void main()
{
    gl_Position = mvp * vec4(vertex_position, 1.0);

    uv = vertex_uv;
    normal = vertex_normal;
}
