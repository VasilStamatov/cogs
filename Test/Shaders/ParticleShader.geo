#version 330 core

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

in VS_OUT
{
	vec4 color;
	float width;
} gs_in[];

out GS_OUT
{
	vec2 uv;
	vec4 color;
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
	
	//get the 4 vertices around the center 
	// glm::vec3 bottomLeft = glm::vec3(-size.x * 0.5f, -size.y * 0.5f, 0.0f);
	// glm::vec3 topLeft = glm::vec3(-size.x * 0.5f, size.y * 0.5f, 0.0f);
	// glm::vec3 topRight = glm::vec3(size.x * 0.5f, size.y * 0.5f, 0.0f);
	// glm::vec3 bottomRight = glm::vec3(size.x * 0.5f, -size.y * 0.5f, 0.0f);

	//bottom left
	pos_world -= (right * gs_in[0].width * 0.5f);
	pos_world.y -= (0.5f * gs_in[0].width);
	gl_Position = PV * vec4(pos_world, 1.0f);
	gs_out.uv = vec2(0.0f, 0.0f);
	gs_out.color = gs_in[0].color;
    EmitVertex();
	
	//bottom right
	pos_world += (right * gs_in[0].width);
	gl_Position = PV * vec4(pos_world, 1.0f);
	gs_out.uv = vec2(1.0f, 0.0f);
	gs_out.color = gs_in[0].color;
	EmitVertex();
	
	//top left
	pos_world -= (right * gs_in[0].width);
	pos_world.y += (1.0f * gs_in[0].width);
	gl_Position = PV * vec4(pos_world, 1.0f);
	gs_out.uv = vec2(0.0f, 1.0f);
	gs_out.color = gs_in[0].color;
	EmitVertex();
	
	//top right
	pos_world += (right * gs_in[0].width);
	gl_Position = PV * vec4(pos_world, 1.0f);
	gs_out.uv = vec2(1.0f, 1.0f);
	gs_out.color = gs_in[0].color;
	EmitVertex();
	
	EndPrimitive();
}