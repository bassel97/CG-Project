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
	unsigned int framebuffer;
	unsigned int textureColorbuffer;
	unsigned int rbo;

	GUI(GameObject * containingObject, Shader* _shader, const char * imagepath);
	//~GUI();

	//main Window constructor
	GUI(GameObject * containingObject, Shader* _shader, unsigned int width, unsigned int height);

	void Start();

	void Update();

	void drawWindow();

	void setTexture(const char * imagepath);

	//Position from -1 to 1
	glm::vec3 position;
	//Scale
	glm::vec3 scale;

private:

	bool isMainWindow;

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