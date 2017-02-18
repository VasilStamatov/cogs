#version 330 core

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

out GS_OUT
{
	vec2 uv;
} gs_out;

uniform mat4 projection;
uniform mat4 view;

void main() 
{
	// get the position of the first vertex (and only one as this is per point)
    vec3 pos_world = gl_in[0].gl_Position.xyz;
	
	mat3 rotMat = mat3(view);
	vec3 d = vec3(view[3][0], view[3][1], view[3][2]);
 
	vec3 cameraPos = -d * rotMat;
	
	//get the direction vector from the vertex to the camera
	vec3 toCamera = normalize(cameraPos - pos_world);
	
	//global up axis
	vec3 up = vec3(0.0f, 1.0f, 0.0f);
	
	//get the right vector of the toCamera direction vector and global up
	vec3 right = cross(up, toCamera);
	
	mat4 PV = projection * view;
	
	//bottom left
	pos_world -= (right * 0.5f);
	pos_world.y -= 0.5f;
	gl_Position = PV * vec4(pos_world, 1.0f);
	gs_out.uv = vec2(0.0f, 0.0f);
    EmitVertex();
	
	//bottom right
	pos_world += right;
	gl_Position = PV * vec4(pos_world, 1.0f);
	gs_out.uv = vec2(1.0f, 0.0f);
	EmitVertex();
	
	//top left
	pos_world -= right;
	pos_world.y += 1.0f;
	gl_Position = PV * vec4(pos_world, 1.0f);
	gs_out.uv = vec2(0.0f, 1.0f);
	EmitVertex();
	
	//top right
	pos_world += right;
	gl_Position = PV * vec4(pos_world, 1.0f);
	gs_out.uv = vec2(1.0f, 1.0f);
	EmitVertex();
	
	EndPrimitive();
}