#version 410

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_colour;
layout(location = 2) in vec3 vertex_normal;
layout(location = 3) in vec2 tex_coord;

out vec3 colour;
out vec2 texcoord;
out vec3 normal;
out vec3 FragPos; 

uniform mat4 transform;
uniform mat4 view;
uniform mat4 projection;

void main() {
	colour =  vertex_colour;
	texcoord = tex_coord;
	normal = vertex_normal;
	FragPos = vec3(transform * vec4(vertex_position, 1.0));
	gl_Position = projection * view * transform * vec4(vertex_position, 1.0);
}
