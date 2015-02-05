#version 400

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 normal;
layout(location = 4) in vec4 vColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 vlightDir;	
uniform vec3 vEye;	
out vec3 eyePos;

out vec3 position_eye, normal_eye;
out vec2 texCoord0;

void main()
{
	position_eye = vec3(view * model * vec4(position,1.0));
	normal_eye = vec3(view * model * vec4(normal,0.0));
	
	eyePos = vec3(view * vec4(vEye, 1.0));
	texCoord0 = texCoord;

	gl_Position = projection * vec4(position_eye,1.0);
}