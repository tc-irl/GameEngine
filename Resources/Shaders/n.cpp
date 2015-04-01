#version 430

layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec4 vColor;
layout(location = 2) in vec2 vTexCoord;
layout(location = 4) in vec3 vNormal;
layout(location = 5) in vec3 vTangent;
layout(location = 6) in vec3 vBitangent;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 vLightDir;
uniform vec3 cameraPos;

out vec3 eyePos, eyeNormal;
out vec2 pTexCoord;
out mat3 tbnMatrix;



// tangent space vector to the light
out vec3 to_light;

//tangent space vector to the eye
out vec3 to_eye;

// fragment position in tangent space
out vec3 position_tan;

// eye space vector to fragment. used for depth correct.
out vec3 eye_to_pos;


void main()
{
	eye_to_pos = vec3 (view * model * vec4(vPosition,1.0));

	mat3 MV3x3 = mat3(view * model);
	
    to_light = tbnMatrix * LightDirection_cameraspace;
    to_eye = tbnMatrix * EyeDirection_cameraspace;

    pTexCoord = vTexCoord;

    position_tan = vec3(vPosition) * tbnMatrix;

	gl_Position = projection * view * model * vec4(vPosition,1.0);
}