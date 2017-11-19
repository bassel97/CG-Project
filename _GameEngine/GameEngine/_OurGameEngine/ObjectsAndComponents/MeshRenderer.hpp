#ifndef MESH_RENDERER
#define MESH_RENDERER

#include <GL/glew.h>
#include <GL\glfw.h>

#include <vector>

#include "_OurGameEngine\Component.hpp"
#include "_OurGameEngine\ObjectsAndComponents\GameObject.hpp"

#include <glm/gtc/type_ptr.hpp>

#include "common\shader.hpp"
#include "common\objloader.hpp"
#include "common\texture.hpp"

class MeshRenderer : public Component
{
public:
	//MeshRenderer();
	//MeshRenderer(GameObject* containingObject);
	MeshRenderer(GameObject * containingObject, std::string modelPath, std::string vertexShaderPath, std::string fragmentShaderPath);
	~MeshRenderer();

	void Start();

	void Update();

	void setTexture(const char * imagepath);

	void Initialize(GameObject * containingObject, GLfloat vertexBufferData[], std::string vertexShaderPath, std::string fragmentShaderPath);




	//for test
	bool hasMVP = false;
	glm::mat4 vp;

private:

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;

	GLuint shadersProgramID;
	GLuint vertexPosition_modelspaceID;
	GLuint color_modelspaceID;
	GLuint transformLocationID;
	GLuint texCoordID;

	GLuint vertexbuffer;
	GLuint colorbuffer;

	GLuint textureID;
	GLuint texCoordBuffer;

	bool hasTexture = false;

};

#endif