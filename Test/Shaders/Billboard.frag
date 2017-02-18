#version 330 core

out vec4 color;

in GS_OUT
{
	vec2 uv;
} fs_in;


uniform sampler2D texture_diffuse;

void main() 
{
	color = texture(texture_diffuse, fs_in.uv);
}