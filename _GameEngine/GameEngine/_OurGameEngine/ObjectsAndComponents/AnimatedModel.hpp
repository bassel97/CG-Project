#ifndef ANIMATED_RENDERER
#define ANIMATED_RENDERER

#include <iostream>

#include <GL/glew.h>
#include <GL\glfw.h>

#include <vector>

#include "_OurGameEngine\Component.hpp"
#include "_OurGameEngine\ObjectsAndComponents\GameObject.hpp"
#include "_OurGameEngine\ObjectsAndComponents\shader.hpp"

#include <glm/gtc/type_ptr.hpp>

#include "common\shader.hpp"
#include "common\objloader.hpp"
#include "common\texture.hpp"

#include <assimp\anim.h>
#include <assimp\scene.h>
#include <assimp\Importer.hpp>
#include <assimp\postprocess.h>

struct Vertex {
	// position
	glm::vec3 Position;
	// normal
	glm::vec3 Normal;
	// texCoords
	glm::vec2 TexCoords;
	// tangent
	glm::vec3 Tangent;
	// bitangent
	glm::vec3 Bitangent;


	//affecting bones
	glm::ivec2 affectingBonesId;
	//Bone weight
	glm::vec2 boneWeights;
};

struct Texture {
	unsigned int id;
	std::string type;
	aiString path;
};

class AnimatedModel : public Component
{
	//string directory;

	std::vector<Texture> textures_loaded;

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;
	unsigned int VAO;

	int bonesNumber;

	std::vector<float> boneWeights;
	std::vector<int> boneIds;

	unsigned int VBO, EBO;

	GLuint shader_vertexPosModelSpaceID;
	GLuint shader_jointIndId;
	GLuint shader_weightsId;

	glm::vec3 testRotaions;
	glm::mat4* BonesTransforms;

public:

	Shader *shader;
	glm::mat4* universalViewProj;

	AnimatedModel(GameObject* contObject, const char* path, Shader* _shader);

	void processAnimation(int animationNum, aiAnimation ** animations);

	glm::vec3 getTestRotaions(float time);


	void processMesh(aiMesh * mesh);
	void processMesh(aiBone ** bones, aiMesh *mesh, int numBones);
	void processBones(aiBone ** bones, aiAnimation** animations, int numBones);


	//std::vector<Texture> loadMaterialTextures(aiMaterial * mat, aiTextureType type, std::string typeName);

	void Draw();

	void Start();

	void Update();
};

#endif