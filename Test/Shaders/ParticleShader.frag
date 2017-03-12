#version 330 core

out vec4 color;

in VS_OUT
{
	vec4 color;
	vec2 uv1;
	vec2 uv2;
	float blend;
} fs_in;


uniform sampler2D texture_diffuse;

void main() 
{
	vec4 color1 = texture(texture_diffuse, fs_in.uv1);
	vec4 color2 = texture(texture_diffuse, fs_in.uv2);
	
	color = fs_in.color * mix(color1, color2, fs_in.blend);
}