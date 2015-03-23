// Helpful tutorials used:
// http://ruh.li/GraphicsOrenNayar.html
// Advanced Lighting and Materials with shaders - Kelly Dempski and Emmanuel Viale. 

#version 400


layout(location = 0) in vec3 position;
layout(location = 1) in vec4 vColor;
layout(location = 2) in vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 vlightDir;	
uniform vec3 vEye;	
out vec3 eyePos;

out vec3 position_eye, normal_eye;
out vec4 color;

void main()
{
	position_eye = vec3(view * model * vec4(position,1.0));
	normal_eye = vec3(view * model * vec4(normal,0.0));
	
	color = vColor;

	gl_Position = projection * vec4(position_eye,1.0);
}