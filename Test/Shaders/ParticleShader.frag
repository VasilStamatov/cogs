#version 330 core

out vec4 color;

in VS_OUT
{
	vec4 color;
	vec2 uv;
} fs_in;


uniform sampler2D texture_diffuse;

void main() 
{
	vec4 textureColor = texture(texture_diffuse, fs_in.uv);
	color = fs_in.color * textureColor;
}