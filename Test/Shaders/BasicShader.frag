#version 330 core

out vec4 color;

in VS_OUT
{
	vec3 position;
	vec2 uv;
	vec4 color;
} fs_in;

struct Material
{
	sampler2D texture_diffuse1;
	sampler2D texture_specular1;
	sampler2D texture_ambient1;
	sampler2D texture_normal1;
	float shininess;
};

uniform Material material;

void main() 
{
	vec4 textureColor = texture(material.texture_diffuse1, fs_in.uv);
    color = fs_in.color * textureColor;
}