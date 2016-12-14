#version 330 core

out vec4 color;

in VS_OUT
{
	vec3 position;
	vec2 uv;
	vec4 color;
} fs_in;

uniform sampler2D spriteTexture;

void main() 
{
	vec4 textureColor = texture(spriteTexture, fs_in.uv);
    color = fs_in.color * textureColor;
}