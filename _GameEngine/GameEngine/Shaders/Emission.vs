#version 330

attribute vec2 aTexCoord;

attribute vec3 vertexPosition_modelspace;
attribute vec3 normal;

uniform mat4 transform;
uniform mat4 projection;
uniform mat4 rotation;

uniform float time;

out vec2 TexCoord;
out vec4 color;
out vec3 normal_vector;
out mat4 rotationMatrix;
out float _time;

void main(){

	_time = time;

	_time *= 5;

	gl_Position =   projection  * transform  * vec4(vertexPosition_modelspace * (abs(sin(_time)/2.0) + 0.5), 1.0);

	normal_vector = normal;
	rotationMatrix = rotation;

	color = rotation * vec4(normal,1);

}