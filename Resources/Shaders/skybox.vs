#version 330 core
layout (location = 0) in vec3 position;
layout (location = 2) in vec3 normal;

out vec3 reflectDir;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform bool DrawSkyBox;
uniform vec3 cameraPosition;

void main()
{	
	if(DrawSkyBox)
	{
		reflectDir = position;
	}
	else
	{
	    vec3 worldPos = vec3(model * vec4(position,1.0) );
        vec3 worldNorm = vec3(model * vec4(normal, 0.0));
        vec3 worldView = normalize(cameraPosition - worldPos);

        reflectDir = reflect(-worldView, worldNorm );
	}
	
	gl_Position = projection * view * model * vec4(position, 1.0);
}
