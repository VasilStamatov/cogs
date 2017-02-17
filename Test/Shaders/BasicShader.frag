#version 330 core

out vec4 color;

in VS_OUT
{
	vec2 uv;
	vec4 color;
} fs_in;


uniform sampler2D texture_diffuse;

void main() 
{
	vec4 textureColor = texture(texture_diffuse, fs_in.uv);
    color = fs_in.color * textureColor;
}