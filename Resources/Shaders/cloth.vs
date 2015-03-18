#version 400
layout(location = 0) in vec3 position;
layout(location = 1) in vec4 vColor;
layout(location = 2) in vec3 vNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec4 color;
out vec3 normal; 

void main()
{
	gl_Position = projection * view * model * vec4(position, 1.0);
	color = vColor;
	normal = vNormal;
}