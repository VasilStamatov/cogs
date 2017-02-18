#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uv;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec3 tangent;

out VS_OUT
{
	vec3 position;
	vec2 uv;
	vec3 cameraPos;
	mat3 TBN;
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

	vs_out.cameraPos = vec3(view[3][0], view[3][1], view[3][2]);
	
    vs_out.uv = uv;
	
	mat3 normalMatrix = transpose(inverse(mat3(model)));	
	
	//the tangent vector
	vec3 T = normalize(normalMatrix * tangent);
	
	vec3 N = normalize(normalMatrix * normal);
	
	// re-orthogonalize T with respect to N
	T = normalize(T - dot(T, N) * N);
	
	//The bitangent vector
	vec3 B = cross(T, N);

	//Send the TBN matrix in order to revert tangent space normal to world space
	vs_out.TBN = mat3(T, B, N);	
}