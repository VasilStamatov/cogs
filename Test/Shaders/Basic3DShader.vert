#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uv;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec3 tangent;

out VS_OUT
{
	vec3 position;
	vec2 uv;
	vec3 normal;
	vec3 tangent;
	vec3 cameraPos;
} vs_out;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main() 
{
    //transform the world space coordinates from the spritebatch to clip coordinates with the ortho projection matrix
    gl_Position = projection * view * model * vec4(position, 1.0);

	//pass the world space coordinates
    vs_out.position = vec3(model * vec4(position, 1.0));

    vs_out.uv = uv;
	
	//pass the world space normals
	vs_out.normal = mat3(transpose(inverse(model))) * normal;	
	
    vs_out.tangent = tangent;
	
	vs_out.cameraPos = vec3(view[3][0], view[3][1], view[3][2]);
}