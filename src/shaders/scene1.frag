#version 410

in vec2 uv;
in vec3 normal;

out vec3 color;

uniform sampler2D samp;

void main()
{
    color = texture(samp, uv).rgb;
}
