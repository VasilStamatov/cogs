#version 330 core

out vec4 color;

in VS_OUT
{
	vec3 position;
	vec2 uv;
	vec3 normal;
	vec3 tangent;
} fs_in;

struct Material
{
	sampler2D texture_diffuse;
	sampler2D texture_specular;
	sampler2D texture_reflection;
	sampler2D texture_normal;
	float shininess;
};

uniform Material material;

void main() 
{
	vec3 sampleColor = vec3(texture(material.texture_diffuse, fs_in.uv));
	// color = vec4(fs_in.uv.x, fs_in.uv.y, 0.0f, 1.0f);
	color = vec4(sampleColor, 1.0f);
}