// Based on Antons OpenGL 4 Tutorials book & BennyBox youtube video: https://www.youtube.com/watch?v=dF5rOveGOJc

#version 400

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 normal;
layout (location = 5) in vec3 vTangent;
layout (location = 6) in vec3 biTangent;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 vLightDir;

out vec2 texCoord0;

out vec3 worldPos0, position_eye, LightDirection_cameraspace;
out vec3 toLightInTangent0, toCameraInTangent0; 
out vec3 EyeDirection_cameraspace;

void main()
{
	worldPos0 = (model * vec4(position, 1.0)).xyz;
	
	position_eye = (view * model * vec4(position, 1.0)).xyz;
	EyeDirection_cameraspace = vec3(0,0,0) - position_eye;

	vec3 LightPosition_cameraspace = (view * vec4(vLightDir,1)).xyz;
	LightDirection_cameraspace = LightPosition_cameraspace + EyeDirection_cameraspace;

	vec3 n = mat3(view * model) * normal;
	vec3 t = mat3(view * model) * vTangent;
	vec3 b = mat3(view * model) * biTangent;

	mat3 tbnMatrix = transpose(mat3(t,b,n));

	toLightInTangent0 = tbnMatrix * LightDirection_cameraspace * tbnMatrix;
	toCameraInTangent0 = tbnMatrix * EyeDirection_cameraspace;

    texCoord0 = texCoord;
	gl_Position = projection * view * model * vec4(position,1.0);

}


