#version 330

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 texCoord0; 

void main()
{
	gl_Position = projection * view * model * vec4(position, 1.0);
	texCoord0 = texCoord;
}