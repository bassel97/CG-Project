#ifndef GUI_COMPONENT
#define GUI_COMPONENT

#include <GL/glew.h>
#include <GL\glfw.h>

#include <vector>

#include "_OurGameEngine\Component.hpp"
#include "_OurGameEngine\ObjectsAndComponents\GameObject.hpp"
#include "_OurGameEngine\ObjectsAndComponents\shader.hpp"

#include <glm/gtc/type_ptr.hpp>

#include "common\objloader.hpp"
#include "common\texture.hpp"

struct UIVertex {
	// position
	glm::vec3 Position;
	// texCoords
	glm::vec2 TexCoords;
};

class GUI : public Component
{
public:

	GUI(GameObject * containingObject, Shader* _shader, const char * imagepath);
	//~GUI();

	void Start();

	void Update();

	void setTexture(const char * imagepath);

	//float vertices[] = {
	//	 positions           // texture coords
	//	1.0f,  1.0f, 0.0f,		1.0f, 1.0f, // top right
	//	1.0f, -1.0f, 0.0f,		1.0f, 0.0f, // bottom right
	//	-1.0f, -1.0f, 0.0f,		0.0f, 0.0f, // bottom left
	//	-1.0f,  1.0f, 0.0f,		0.0f, 1.0f  // top left 
	//};
	//unsigned int indices[] = {
	//	0, 1, 3, // first triangle
	//	1, 2, 3  // second triangle
	//};

	//Position from -1 to 1
	glm::vec3 position;
	//Scale
	glm::vec3 scale;

private:

	unsigned int VBO, VAO, EBO;

	unsigned int imageID;

	glm::mat4 transform;

	GLuint textureID;
	GLuint texCoordBuffer;

	Shader* shader;

	bool hasTexture = false;

	std::vector<UIVertex> verts;
	std::vector<unsigned int> indices;
};

#endif