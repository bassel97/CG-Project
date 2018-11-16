#include "GUI.hpp"

GUI::GUI(GameObject * containingObject, Shader* _shader, const char * imagepath)
	:Component(containingObject)
{
	isMainWindow = false;

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

GUI::GUI(GameObject * containingObject, Shader * _shader, unsigned int width, unsigned int height)
	:Component(containingObject)
{
	isMainWindow = true;

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

	// framebuffer configuration
	// -------------------------
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	// create a color attachment texture
	glGenTextures(1, &textureColorbuffer);
	glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
	// create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height); // use a single renderbuffer object for both a depth AND stencil buffer.
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it
																								  // now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	else
		std::cout << "Framebuffer is complete! " << framebuffer << std::endl;
}

void GUI::Start()
{
}

void GUI::Update()
{
	if (!isMainWindow) {
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
}

void GUI::drawWindow()
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
	glBindTexture(GL_TEXTURE_2D, textureColorbuffer);

	// draw mesh
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void GUI::setTexture(const char * imagepath)
{
	imageID = loadBMP_custom(imagepath);
}
