#include "MeshRenderer.hpp"

//MeshRenderer::MeshRenderer()
//{
//}

//MeshRenderer::MeshRenderer(GameObject *containingObject)
//{
//}

MeshRenderer::MeshRenderer(GameObject * containingObject, std::string modelPath, std::string vertexShaderPath, std::string fragmentShaderPath)
	:Component(containingObject)
{
	shadersProgramID = LoadShaders(vertexShaderPath.c_str(), fragmentShaderPath.c_str());

	vertexPosition_modelspaceID = glGetAttribLocation(shadersProgramID, "vertexPosition_modelspace");
	color_modelspaceID = glGetAttribLocation(shadersProgramID, "color_modelspace");
	texCoordID = glGetAttribLocation(shadersProgramID, "aTexCoord");

	if (!loadOBJ(modelPath.c_str(), vertices, uvs, normals)) {
		printf("Failed");
		return;
	}

	int sizeOF = vertices.size() * sizeof(glm::vec3);

	glGenBuffers(1, &vertexbuffer);//Generate 1 buffer, put the resulting identifier in vertexbuffer
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);//First argument specifies the target to which the buffer object is bound.
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);//glBufferData( GLenum   target,  GLsizeiptr   size,  const GLvoid *   data,  GLenum   usage)
																									  //tareget->specifies target buffer object
																									  //size->specifies the size of the buffer
																									  //data->specifies Specifies a pointer to data that will be copied into the data store	
																									  //usage->is a hint to the GL implementation a


	glGenBuffers(1, &colorbuffer);//Generate 1 buffer, put the resulting identifier in vertexbuffer
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);//First argument specifies the target to which the buffer object is bound.
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);//glBufferData( GLenum   target,  GLsizeiptr   size,  const GLvoid *   data,  GLenum   usage)
																									 //tareget->specifies target buffer object
																									 //size->specifies the size of the buffer
																									 //data->specifies Specifies a pointer to data that will be copied into the data store	
																									 //usage->is a hint to the GL implementation a

}

MeshRenderer::~MeshRenderer()
{
}

void MeshRenderer::Start()
{
}

void MeshRenderer::Update()
{

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	glm::mat4 Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
	// Camera matrix
	glm::mat4 View = glm::lookAt(
		glm::vec3(4, 3, -3), // Camera is at (4,3,-3), in World Space (6,1.5,0)
		glm::vec3(0, 0, 0), // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
	);



	// Use our shader
	glUseProgram(shadersProgramID);

	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(vertexPosition_modelspaceID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribPointer(
		vertexPosition_modelspaceID, // The attribute we want to configure
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?(i.e. map to -1 and 1)
		0,                  // stride: specifies the byte stride from one attribute to the next
		(void*)0            // array buffer offset: start from the beginning of the buffer
	);

	glEnableVertexAttribArray(color_modelspaceID);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glVertexAttribPointer(
		color_modelspaceID, // The attribute we want to configure
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?(i.e. map to -1 and 1)
		0,                  // stride: specifies the byte stride from one attribute to the next
		(void*)0            // array buffer offset: start from the beginning of the buffer
	);

	if (containingObject) {
		transformLocationID = glGetUniformLocation(shadersProgramID, "transform");

		glm::mat4 transformMatrix = containingObject->getTransformMatrix();


		glm::mat4 MVP = Projection * View * transformMatrix; // Remember, matrix multiplication is the other way around


		if (hasMVP) {
			//MVP = vp * transformMatrix;
			glUniformMatrix4fv(glGetUniformLocation(shadersProgramID, "projection"), 1, GL_FALSE, glm::value_ptr(vp));
		}

		glUniformMatrix4fv(transformLocationID, 1, GL_FALSE, glm::value_ptr(transformMatrix));
	}

	if (hasTexture) {

		glEnableVertexAttribArray(texCoordID);
		glBindBuffer(GL_ARRAY_BUFFER, texCoordBuffer);
		glVertexAttribPointer(
			texCoordID, // The attribute we want to configure
			2,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?(i.e. map to -1 and 1)
			0,                  // stride: specifies the byte stride from one attribute to the next
			(void*)0            // array buffer offset: start from the beginning of the buffer
		);

		glBindTexture(GL_TEXTURE_2D, textureID);
	}

	// Draw the triangle !
	glDrawArrays(GL_TRIANGLES, 0, vertices.size()); // 3 indices starting at 0 -> 1 triangle

	glDisableVertexAttribArray(vertexPosition_modelspaceID);

}

void MeshRenderer::setTexture(const char * imagepath) {
	hasTexture = true;

	textureID = loadBMP_custom(imagepath);

	glGenBuffers(1, &texCoordBuffer);//Generate 1 buffer, put the resulting identifier in vertexbuffer
	glBindBuffer(GL_ARRAY_BUFFER, texCoordBuffer);//First argument specifies the target to which the buffer object is bound.
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);//glBufferData( GLenum   target,  GLsizeiptr   size,  const GLvoid *   data,  GLenum   usage)
																						   //tareget->specifies target buffer object
																						   //size->specifies the size of the buffer
																						   //data->specifies Specifies a pointer to data that will be copied into the data store	
																						   //usage->is a hint to the GL implementation a

}

//If default Constructor was called
//used in arrays
void MeshRenderer::Initialize(GameObject * containingObject, GLfloat vertexBufferData[], std::string vertexShaderPath, std::string fragmentShaderPath)
{
	static const GLfloat g_vertex_buffer_data[] = {
		-0.1f, -0.1f, 0.0f,
		0.1f,  -0.1f, 0.0f,
		0.0f,  0.1f, 0.0f,
	};

	this->containingObject = containingObject;

	shadersProgramID = LoadShaders(vertexShaderPath.c_str(), fragmentShaderPath.c_str());

	vertexPosition_modelspaceID = glGetAttribLocation(shadersProgramID, "vertexPosition_modelspace");



	glGenBuffers(1, &vertexbuffer);//Generate 1 buffer, put the resulting identifier in vertexbuffer
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);//First argument specifies the target to which the buffer object is bound.
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);//glBufferData( GLenum   target,  GLsizeiptr   size,  const GLvoid *   data,  GLenum   usage)
																									  //tareget->specifies target buffer object
																									  //size->specifies the size of the buffer
																									  //data->specifies Specifies a pointer to data that will be copied into the data store	
																									  //usage->is a hint to the GL implementation a


}
