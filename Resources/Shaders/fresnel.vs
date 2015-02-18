#version 410

layout (location = 0) in vec3 position;
layout (location = 2) in vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 position_eye, normal_eye;

void main () 
{
	position_eye = vec3 (view * model * vec4 (position, 1.0));
	normal_eye = vec3 (view * model * vec4 (normal, 0.0));
	gl_Position = projection * view * model * vec4 (position, 1.0);
}
