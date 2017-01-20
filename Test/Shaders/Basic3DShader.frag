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
	sampler2D texture_diffuse1;
	sampler2D texture_specular1;
	sampler2D texture_ambient1;
	sampler2D texture_normal1;
	float shininess;
};

uniform Material material;

void main() 
{
	vec3 sampleColor = vec3(texture(material.texture_diffuse1, fs_in.uv));
	color = vec4(fs_in.uv.x, fs_in.uv.y, 0.0f, 1.0f);
	// color = vec4(sampleColor, 1.0f);
}