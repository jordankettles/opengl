#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec2 vertexUV;
layout(location = 2) in vec3 vertexNormal_modelspace;

// Output data ; will be interpolated for each fragment.
out vec2 UV;
out vec3 posWorldSpace; //Position of the vertex in worldspace.
out vec3 normalCameraSpace;
out vec3 eyeDirectionCameraSpace;
out vec3 lightDirectionCameraSpace;

// Values that stay constant for the whole mesh.
uniform mat4 MVP;
uniform mat4 V;
uniform mat4 M;
uniform vec3 lightPositionWorldSpace;


void main(){
	
	// Output position of the vertex, in clip space : MVP * position
	gl_Position =  MVP * vec4(vertexPosition_modelspace,1);

	UV = vertexUV; //UV of the vertex.

	//Position of the vertex in worldspace.
	posWorldSpace = (M * vec4(vertexPosition_modelspace, 1)).xyz;

	//Vector from the vertex to the camera in Camera Space.
	vec3 vertexPosCameraSpace = (V * M * vec4(vertexPosition_modelspace, 1)).xyz;
	eyeDirectionCameraSpace = vec3(0, 0, 0) - vertexPosCameraSpace;

	//Vector from the vertex to the light in Camera Space.
	vec3 lightPositionCameraSpace = (V * vec4(lightPositionWorldSpace, 1)).xyz;
	lightDirectionCameraSpace = lightPositionCameraSpace + eyeDirectionCameraSpace;

	//Normal of the vertex in Camera Space.
	normalCameraSpace = (V * M * vec4(vertexNormal_modelspace, 0)).xyz;
	//Use the inverse transpose if Model Matrix does not scale the model??? how do i know that? 19th may

}

