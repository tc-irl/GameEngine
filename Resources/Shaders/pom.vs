// Based on Antons OpenGL 4 Tutorials book & BennyBox youtube video: https://www.youtube.com/watch?v=dF5rOveGOJc

#version 400

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 normal;
layout (location = 5) in vec3 vTangent;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 normal_mat;

out vec3 position_eye, normal_eye;
out vec2 texCoord0;
out vec3 worldPos0;
out mat3 tbnMatrix;

uniform vec3 vLightDir;
uniform vec3 vEye;
uniform vec3 cameraPosition;

out vec3 toLight;
out vec3 toEye;

void main()
{
	position_eye = vec3 (view * model * vec4 (position, 1.0));
	worldPos0 = vec3 (model * vec4(position, 1.0));
	// vec3 L = normalize(vLightDir - worldPos0);
	// vec3 E = normalize(-position_eye);

	vec3 N = mat3(normal_mat) * normal;
	vec3 T = mat3(normal_mat) * vTangent;

	vec3 B = cross(N, T);

	// toEye   = vec3(dot(E,T), dot(E,B), dot(E,N));
	// toLight = vec3(dot(L,T), dot(L,B), dot(L,N));


		// position_eye = vec3 (view * model * vec4 (position, 1.0));
	// worldPos0 = vec3 (model * vec4(position, 1.0));
	vec3 L = normalize(vLightDir - worldPos0);
	vec3 E = normalize(position_eye - worldPos0);

	vec3 n = normalize((model * vec4(normal,0.0)).xyz);
	vec3 t = normalize((model * vec4(vTangent,0.0)).xyz);

	t = normalize(t - dot(t,n) * n);

	vec3 biTangent = cross(t, n);
	tbnMatrix = mat3(t,biTangent,n);

	// tbnMatrix = mat3(T,B,N);

	toEye   = E;
	toLight = L;

	// DO NOT CHANGE MORON

	texCoord0 = texCoord; 

	gl_Position = projection * view * model * vec4(position,1.0);
}


