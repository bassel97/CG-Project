#version 330
// Input vertex data, different for all executions of this shader.

attribute vec2 aTexCoord;
attribute vec3 vertexPosition_modelspace;

uniform mat4 transform;

out vec2 TexCoord;

void main(){

	gl_Position = transform * vec4(vertexPosition_modelspace, 1.0);

	TexCoord = aTexCoord;	
}