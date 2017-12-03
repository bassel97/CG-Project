#version 330
// Input vertex data, different for all executions of this shader.


attribute vec2 aTexCoord;
//attribute vec2 bone_Weight;
attribute float boneInd;

attribute vec3 vertexPosition_modelspace;
attribute vec3 normal;

uniform mat4 transform;
uniform mat4 projection;

const int maxTransforms = 50;
uniform mat4 joints[maxTransforms];
uniform int numBones;

out vec2 TexCoord;

void main(){

	//if(boneInd == -1){
		//gl_Position = projection * transform * vec4(vertexPosition_modelspace, 1.0);
	//}

	for (int i = 0; i < numBones; i++){
		if (i == boneInd){
			gl_Position =   projection  * transform * joints[i]  * vec4(vertexPosition_modelspace, 1.0);
		}
	}

	TexCoord = aTexCoord;	
}