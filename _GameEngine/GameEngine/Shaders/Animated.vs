#version 330
// Input vertex data, different for all executions of this shader.

attribute vec2 aTexCoord;
attribute vec3 vertexPosition_modelspace;

attribute ivec2 jointInd;
attribute vec2 weights;

uniform mat4 transform;
uniform mat4 projection;

out vec4 myColor;
out vec2 TexCoord;

void main(){

	gl_Position = projection * transform * vec4(vertexPosition_modelspace, 1.0);

	myColor = vec4(vertexPosition_modelspace, 1.0);

	TexCoord = aTexCoord;	
}