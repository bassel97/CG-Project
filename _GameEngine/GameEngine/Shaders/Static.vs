#version 330
// Input vertex data, different for all executions of this shader.

attribute vec2 aTexCoord;
attribute vec3 vertexPosition_modelspace;
attribute vec3 normal;

uniform mat4 transform;
uniform mat4 projection;
uniform mat4 rotation;

out vec4 myColor;
out vec2 TexCoord;
out vec3 normal_vector;
out mat4 rotationMatrix;

void main(){

	gl_Position = projection * transform * vec4(vertexPosition_modelspace, 1.0);

	myColor = vec4(vertexPosition_modelspace, 1.0);

	TexCoord = aTexCoord;
	normal_vector = normal;
	rotationMatrix = rotation;	
}