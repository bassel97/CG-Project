#include "GUI.hpp"

GUI::GUI(GameObject * containingObject, Shader* _shader, const char * imagepath)
	:Component(containingObject)
{
	shader = _shader;
	shader->use();

	GLuint shader_vertexPosModelSpaceID = glGetAttribLocation(shader->ID, "vertexPosition_modelspace");
	GLuint shader_textureCoordinateID = glGetAttribLocation(shader->ID, "aTexCoord");

	UIVertex v;
	v.Position = glm::vec3(1.0f, 1.0f, 0.0f);
	v.TexCoords = glm::vec2(1.0f, 1.0f);
	verts.push_back(v);

	UIVertex v1;
	v1.Position = glm::vec3(1.0f, -1.0f, 0.0f);
	v1.TexCoords = glm::vec2(1.0f, 0.0f);
	verts.push_back(v1);

	UIVertex v2;
	v2.Position = glm::vec3(-1.0f, -1.0f, 0.0f);
	v2.TexCoords = glm::vec2(0.0f, 0.0f);
	verts.push_back(v2);

	UIVertex v3;
	v3.Position = glm::vec3(-1.0f, 1.0f, 0.0f);
	v3.TexCoords = glm::vec2(0.0f, 1.0f);
	verts.push_back(v3);

	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(3);
	indices.push_back(1);
	indices.push_back(2);
	indices.push_back(3);

	transform = glm::mat4(1.0f);
	scale = glm::vec3(1.0, 1.0, 1.0);


	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(UIVertex), &verts[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	// position attribute
	glEnableVertexAttribArray(shader_vertexPosModelSpaceID);
	glVertexAttribPointer(shader_vertexPosModelSpaceID, 3, GL_FLOAT, GL_FALSE, sizeof(UIVertex), (void*)offsetof(UIVertex, Position));

	// texture coord attribute
	glEnableVertexAttribArray(shader_textureCoordinateID);
	glVertexAttribPointer(shader_textureCoordinateID, 2, GL_FLOAT, GL_FALSE, sizeof(UIVertex), (void*)offsetof(UIVertex, TexCoords));


	setTexture(imagepath);
}

void GUI::Start()
{
}

void GUI::Update()
{
	shader->use();

	transform = glm::translate(position);
	transform = transform * glm::scale(scale);

	shader->setMat4("transform", transform);

	glActiveTexture(GL_TEXTURE0);
	std::string name = "Image";
	GLuint i = 0;
	glUniform1i(glGetUniformLocation(shader->ID, name.c_str()), 0);
	// and finally bind the texture
	glBindTexture(GL_TEXTURE_2D, imageID);

	// draw mesh
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void GUI::setTexture(const char * imagepath)
{
	imageID = loadBMP_custom(imagepath);
}
