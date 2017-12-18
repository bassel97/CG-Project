#include "AnimatedModel.hpp"

//fmod music library

//radias probs 

//yahiazakaria13@gmail.com

aiNodeAnim* FindNodeAnim(aiAnimation* pAnimation, std::string NodeName)
{
	for (int i = 0; i < pAnimation->mNumChannels; i++) {
		aiNodeAnim* pNodeAnim = pAnimation->mChannels[i];
		std::string pNodeAnimName = pAnimation->mChannels[i]->mNodeName.C_Str();

		if (pNodeAnimName == NodeName) return pNodeAnim;
	}

	return NULL;
}

AnimatedModel::AnimatedModel(GameObject* contObject, const char* path, Shader* _shader) :Component(contObject) {

	camRef = NULL;
	walking = false;

	shader = _shader;

	//aiProcess_FlipUVs
	scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_CalcTangentSpace);
	// check for errors
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
	{
		std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
		return;
	}

	shader->use();

	shader_vertexPosModelSpaceID = glGetAttribLocation(shader->ID, "vertexPosition_modelspace");
	shader_textureCoordinateID = glGetAttribLocation(shader->ID, "aTexCoord");
	shader_normalID = glGetAttribLocation(shader->ID, "normal");

	bonesNumber = 0;

	if (scene->mMeshes[0]->HasBones()) {

		for (int i = 0; i < scene->mMeshes[0]->mNumBones; i++)
		{
			printf(scene->mMeshes[0]->mBones[i]->mName.C_Str());
			printf("\n");
		}
		/*
		aiMatrix4x4 firstBoneMatrixAI;
		aiNode* boneNode = scene->mRootNode->FindNode(scene->mMeshes[0]->mBones[0]->mName.C_Str());
		aiNode* traverseNode = scene->mRootNode;
		while (boneNode != traverseNode) {
			firstBoneMatrixAI *= traverseNode->mTransformation;
			traverseNode = traverseNode->mChildren[0];
		}
		glm::mat4 firstBoneMatrix;
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				firstBoneMatrix[i][j] = firstBoneMatrixAI[i][j];
			}
		}*/

		processMesh(scene->mMeshes[0]->mBones, scene->mMeshes[0], scene->mMeshes[0]->mNumBones);
		//processBones(scene->mMeshes[0]->mBones, scene->mRootNode, scene->mMeshes[0]->mBones[0], glm::mat4());
	}
	else
		processMesh(scene->mMeshes[0]);

	//processBones(scene->mMeshes[0]->mBones, scene->mAnimations, scene->mMeshes[0]->mNumBones);

	if (bonesNumber > 0)
		processAnimation(scene->mNumAnimations, scene->mAnimations);
}

void AnimatedModel::setTexture(const char * imagepath, std::string type) {
	Texture t;

	t.id = loadBMP_custom(imagepath);
	t.type = type;

	textures.push_back(t);
}

void AnimatedModel::processAnimation(int animationNum, aiAnimation ** animations) {
	for (int i = 0; i < animationNum; i++)
	{
		for (int j = 0; j < animations[i]->mNumChannels; j++)
		{
			aiNodeAnim* a = animations[i]->mChannels[j];
			std::cout << a->mNodeName.C_Str() << std::endl;
			std::cout << a->mNumRotationKeys << std::endl;
			for (int k = 0; k < a->mNumRotationKeys; k++)
			{
				std::cout << "KeyFrame : " << k << std::endl;
				aiMatrix3x3 axs = a->mRotationKeys[k].mValue.GetMatrix();

				std::cout << "Scale :" << a->mScalingKeys[k].mValue.x << " " << a->mScalingKeys[k].mValue.y << " " << a->mScalingKeys[k].mValue.z << std::endl;
				std::cout << "Rotation :" << a->mRotationKeys[k].mValue.x << " " << a->mRotationKeys[k].mValue.y << " " << a->mRotationKeys[k].mValue.z << " " << a->mRotationKeys[k].mValue.w << std::endl;
				std::cout << "translate :" << a->mPositionKeys[k].mValue.x << " " << a->mPositionKeys[k].mValue.y << " " << a->mPositionKeys[k].mValue.z << std::endl;

				if (k == 15) {
					testRotaions.x = a->mRotationKeys[k].mValue.x;
					testRotaions.y = a->mRotationKeys[k].mValue.y;
					testRotaions.z = a->mRotationKeys[k].mValue.z;
				}
			}
		}
	}
}

glm::vec3 AnimatedModel::getTestRotaions(float time) {
	return testRotaions;
}

void AnimatedModel::processMesh(aiMesh *mesh)
{
	// Walk through each of the mesh's vertices
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
						  // positions
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.Position = vector;
		// normals
		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		vertex.Normal = vector;
		// texture coordinates
		if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
		{
			glm::vec2 vec;
			// a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
			// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = vec;
		}
		else
			vertex.TexCoords = glm::vec2(0.0f, 0.0f);
		// tangent
		vector.x = mesh->mTangents[i].x;
		vector.y = mesh->mTangents[i].y;
		vector.z = mesh->mTangents[i].z;
		vertex.Tangent = vector;
		// bitangent
		vector.x = mesh->mBitangents[i].x;
		vector.y = mesh->mBitangents[i].y;
		vector.z = mesh->mBitangents[i].z;
		vertex.Bitangent = vector;
		vertices.push_back(vertex);
	}
	// now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		// retrieve all indices of the face and store them in the indices vector
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	//return Mesh(vertices, indices, textures);

	// create buffers/arrays
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	// load data into vertex buffers
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// A great thing about structs is that their memory layout is sequential for all its items.
	// The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
	// again translates to 3/2 floats which translates to a byte array.
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	// set the vertex attribute pointers
	// vertex Positions
	glEnableVertexAttribArray(shader_vertexPosModelSpaceID);
	glVertexAttribPointer(shader_vertexPosModelSpaceID, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	// vertex normals
	glEnableVertexAttribArray(shader_normalID);
	glVertexAttribPointer(shader_normalID, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	// vertex texture coords
	glEnableVertexAttribArray(shader_textureCoordinateID);
	glVertexAttribPointer(shader_textureCoordinateID, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
	// vertex tangent
	//glEnableVertexAttribArray(3);
	//glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
	// vertex bitangent
	//glEnableVertexAttribArray(4);
	//glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));

	// Weights bitangent
	//glEnableVertexAttribArray(shader_jointIndId);
	//glVertexAttribPointer(shader_jointIndId, 2, GL_INT, GL_FALSE, sizeof(float), (void*)offsetof(Vertex, Bitangent));

	// Indexes bitangent
	//glEnableVertexAttribArray(shader_weightsId);
	//glVertexAttribPointer(shader_weightsId, 2, GL_FLOAT, GL_FALSE, sizeof(float), (void*)offsetof(Vertex, Bitangent));

	glBindVertexArray(0);
}

void AnimatedModel::processMesh(aiBone ** bones, aiMesh *mesh, int numBones)
{
	bonesNumber = numBones;
	BonesTransforms = new glm::mat4[numBones];

	//shader_weightsId = glGetAttribLocation(shader->ID, "bone_Weight");
	shader_jointIndId = glGetAttribLocation(shader->ID, "boneInd");

	// Walk through each of the mesh's vertices
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
						  // positions
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.Position = vector;
		// normals
		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		vertex.Normal = vector;
		// texture coordinates
		if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
		{
			glm::vec2 vec;
			// a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
			// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = vec;
		}
		else
			vertex.TexCoords = glm::vec2(0.0f, 0.0f);
		// tangent
		vector.x = mesh->mTangents[i].x;
		vector.y = mesh->mTangents[i].y;
		vector.z = mesh->mTangents[i].z;
		vertex.Tangent = vector;
		// bitangent
		vector.x = mesh->mBitangents[i].x;
		vector.y = mesh->mBitangents[i].y;
		vector.z = mesh->mBitangents[i].z;
		vertex.Bitangent = vector;


		int numIds = 0;
		//glm::vec2 weights = glm::vec2(-1, -1);
		//glm::ivec2 ids = glm::ivec2(-1, -1);
		int id = 0;

		float maxWeight = 0;
		//Setting the bone index foreach vertex
		for (int j = 0; j < numBones; j++)
		{
			for (int k = 0; k < bones[j]->mNumWeights; k++)
			{
				if (bones[j]->mWeights[k].mVertexId == i) {
					if (bones[j]->mWeights[k].mWeight > maxWeight) {
						maxWeight = bones[j]->mWeights[k].mWeight;
						id = j;
					}
				}
			}
		}

		//vertex.boneWeights = weights;
		//printf("\n\nThe vertex %d has Affecting id = %d with weight = %f", i, id, maxWeight);
		vertex.affectingBoneId = id;


		vertices.push_back(vertex);
	}
	// now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		// retrieve all indices of the face and store them in the indices vector
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	//return Mesh(vertices, indices, textures);

	// create buffers/arrays
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	// load data into vertex buffers
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// A great thing about structs is that their memory layout is sequential for all its items.
	// The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
	// again translates to 3/2 floats which translates to a byte array.
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	// set the vertex attribute pointers
	// vertex Positions
	glEnableVertexAttribArray(shader_vertexPosModelSpaceID);
	glVertexAttribPointer(shader_vertexPosModelSpaceID, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	// vertex normals
	glEnableVertexAttribArray(shader_normalID);
	glVertexAttribPointer(shader_normalID, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	// vertex texture coords
	glEnableVertexAttribArray(shader_textureCoordinateID);
	glVertexAttribPointer(shader_textureCoordinateID, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
	// vertex tangent
	//glEnableVertexAttribArray(3);
	//glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
	// vertex bitangent
	//glEnableVertexAttribArray(4);
	//glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));

	// Weights bitangent
	glEnableVertexAttribArray(shader_jointIndId);
	glVertexAttribPointer(shader_jointIndId, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, affectingBoneId));

	// Indexes bitangent
	//glEnableVertexAttribArray(shader_weightsId);
	//glVertexAttribPointer(shader_weightsId, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, boneWeights));

	glBindVertexArray(0);

	for (int i = 0; i < bonesNumber; i++)
	{
		BonesTransforms[i] = glm::scale(glm::mat4(), 1.0f, 1.0f, 1.0f);
	}
}

void AnimatedModel::processBones(aiBone** bones, aiNode* rootNode, aiBone* FirstBone, glm::mat4 parentMatrix)
{
	//printf(FirstBone->mName.C_Str());
	//printf("     ==============================ParentMatrix\n");
	/*for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			printf("%f ,", parentMatrix[i][j]);
		}
		printf("\n");
	}*/

	glm::mat4 boneMatrix;

	aiNode *node = rootNode->FindNode(FirstBone->mName);

	//printf(node->mName.C_Str());
	//printf(" from parent\n");

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			boneMatrix[i][j] = node->mTransformation[i][j];
			//printf("%f ,", node->mTransformation[i][j]);
		}
		//printf("\n");
	}

	/*printf(node->mName.C_Str());
	printf(" from origin\n");
	boneMatrix = parentMatrix * boneMatrix;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			printf("%f ,", boneMatrix[i][j]);
		}
		printf("\n");
	}*/

	glm::mat4 rotated = glm::transpose(parentMatrix);// = glm::inverse(glm::transpose(parentMatrix));

	if (strcmp(FirstBone->mName.C_Str(), "spine.002") == 0) {
		//rotated = glm::translate(glm::transpose(parentMatrix), glm::vec3(0, sin((float)glfwGetTime() * 2) * 0.05f, 0));
	}

	//Walk Animation ====================================================//
	if (walking) {
		float sinVal = sin((float)glfwGetTime() * 10);

		if (sinVal > 0) {
			if (strcmp(FirstBone->mName.C_Str(), "heel.02.L") == 0) {
				//rotated = boneMatrix * glm::transpose(glm::rotate(glm::mat4(), 45.0f, glm::vec3(0, 0, 1))) * glm::inverse(boneMatrix);
				//rotated = glm::transpose(boneMatrix) * glm::transpose(glm::translate(glm::mat4(), glm::vec3(1, 0, 0)))  * glm::transpose(glm::inverse(boneMatrix));
				//rotated = glm::translate(glm::transpose(boneMatrix), glm::vec3(0, 0, 0));
				//rotated = glm::rotate(glm::transpose(boneMatrix), 45.0f, glm::vec3(0, 0, 1));

				//rotated = glm::rotate(glm::transpose(parentMatrix), sin((float)glfwGetTime() * 10) * 25.0f, glm::vec3(1, 1, 0));
				rotated = glm::translate(glm::mat4(), glm::vec3(0, sinVal*0.25f, sinVal * 0.1f));
			}
			if (strcmp(FirstBone->mName.C_Str(), "toe.L") == 0) {
				rotated = glm::translate(glm::mat4(), glm::vec3(0, sinVal * 0.25f, sinVal * 0.1f));
			}
			if (strcmp(FirstBone->mName.C_Str(), "foot.L") == 0) {
				rotated = glm::translate(glm::mat4(), glm::vec3(0, sinVal * 0.25f, sinVal * 0.1f));
			}
			if (strcmp(FirstBone->mName.C_Str(), "shin.L") == 0) {
				rotated = glm::translate(glm::mat4(), glm::vec3(0, sinVal * 0.25f, sinVal * 0.1f));
			}
			if (strcmp(FirstBone->mName.C_Str(), "thigh.L") == 0) {
				rotated = glm::translate(glm::mat4(), glm::vec3(0, sinVal * 0.1f, sinVal * 0.1f));
			}
		}
		else {
			sinVal = -sinVal;
			if (strcmp(FirstBone->mName.C_Str(), "heel.02.R") == 0) {
				rotated = glm::translate(glm::mat4(), glm::vec3(0, sinVal * 0.25f, sinVal* 0.1f));
			}
			if (strcmp(FirstBone->mName.C_Str(), "toe.R") == 0) {
				rotated = glm::translate(glm::mat4(), glm::vec3(0, sinVal * 0.25f, sinVal* 0.1f));
			}
			if (strcmp(FirstBone->mName.C_Str(), "foot.R") == 0) {
				rotated = glm::translate(glm::mat4(), glm::vec3(0, sinVal * 0.25f, sinVal* 0.1f));
			}
			if (strcmp(FirstBone->mName.C_Str(), "shin.R") == 0) {
				rotated = glm::translate(glm::mat4(), glm::vec3(0, sinVal * 0.25f, sinVal* 0.1f));
			}
			if (strcmp(FirstBone->mName.C_Str(), "thigh.R") == 0) {
				rotated = glm::translate(glm::mat4(), glm::vec3(0, sinVal * 0.1f, sinVal * 0.1f));
			}
		}
	}
	//Walk Animation ====================================================//


	if (strcmp(FirstBone->mName.C_Str(), "forearm.R") == 0) {
		//rotated = boneMatrix * glm::transpose(glm::rotate(glm::mat4(), 45.0f, glm::vec3(0, 0, 1))) * glm::inverse(boneMatrix);
		//rotated = glm::transpose(boneMatrix) * glm::transpose(glm::translate(glm::mat4(), glm::vec3(1, 0, 0)))  * glm::transpose(glm::inverse(boneMatrix));
		//rotated = glm::translate(glm::transpose(boneMatrix), glm::vec3(0, 0, 0));
		//rotated = glm::rotate(glm::transpose(boneMatrix), 45.0f, glm::vec3(0, 0, 1));
		rotated = glm::rotate(glm::transpose(parentMatrix), sin((float)glfwGetTime() * 10) * -7.0f, glm::vec3(0, 1, 0));
	}

	if (strcmp(FirstBone->mName.C_Str(), "upper_arm.L") == 0) {
		//rotated = boneMatrix * glm::transpose(glm::rotate(glm::mat4(), 45.0f, glm::vec3(0, 0, 1))) * glm::inverse(boneMatrix);
		//rotated = glm::transpose(boneMatrix) * glm::transpose(glm::translate(glm::mat4(), glm::vec3(1, 0, 0)))  * glm::transpose(glm::inverse(boneMatrix));
		//rotated = glm::translate(glm::transpose(boneMatrix), glm::vec3(0, 0, 0));
		//rotated = glm::rotate(glm::transpose(boneMatrix), 45.0f, glm::vec3(0, 0, 1));
		rotated = glm::rotate(glm::transpose(parentMatrix), sin((float)glfwGetTime() * 10) * 7.0f, glm::vec3(0, 1, 0));
	}

	/*printf("Rotated matrix\n");
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			printf("%f ,", rotated[i][j]);
		}
		printf("\n");
	}*/

	for (int i = 0; i < bonesNumber; i++)
	{
		if (bones[i]->mName == FirstBone->mName)
		{
			BonesTransforms[i] = rotated;
			//printf("%d", i);
			//printf(" Passed to shader\n");
			break;
		}
	}

	for (int i = 0; i < node->mNumChildren; i++)
	{
		aiMatrix4x4 passed = node->mChildren[i]->mTransformation;
		glm::mat4 sentMatrix;
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				sentMatrix[i][j] = passed[i][j];
			}
		}
		processBones(bones, node, (aiBone*)node->mChildren[i], rotated);
	}

}

void AnimatedModel::Draw()
{
	if (containingObject->freeze > 0)
		containingObject->freeze -= 0.0008;

	if (camRef) {
		glm::vec3 camOut = camRef->getForwardVector();

		//std::cout << containingObject->name << " " << camOut.x << " " << camOut.y << " " << camOut.z << " \n";

		//shader->setVec3("camDirection", 1.0f , 1.0f, 1.0f);
		shader->setVec3("camDirection", camOut);
	}

	shader->use();

	glm::mat4 transform = containingObject->getTransformMatrix();
	glm::mat4 rotationMatrix = containingObject->getRotationMatrix();

	// view/projection transformations
	shader->setMat4("rotation", rotationMatrix);
	shader->setMat4("transform", transform);
	shader->setMat4("projection", (*universalViewProj));

	shader->setFloat("time", (float)glfwGetTime());
	shader->setFloat("freeze", containingObject->freeze);

	//shader->setVec4("tests[0]", glm::vec4(sin(glfwGetTime()), 0, 0, 0));
	//shader->setVec4("tests[1]", glm::vec4(0, cos(glfwGetTime()), 0, 0));

	if (bonesNumber > 0) {
		shader->setInt("numBones", bonesNumber);

		processBones(scene->mMeshes[0]->mBones, scene->mRootNode, scene->mMeshes[0]->mBones[0], glm::mat4());

		for (int i = 0; i < bonesNumber; i++)
		{
			shader->setMat4("joints[" + std::to_string(i) + "]", BonesTransforms[i]);
		}
	}



	//bind appropriate textures
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	unsigned int normalNr = 1;
	unsigned int heightNr = 1;
	for (unsigned int i = 0; i < textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
										  // retrieve texture number (the N in diffuse_textureN)
		//std::string number;
		std::string name = textures[i].type;
		//if (name == "texture_diffuse")
		//	number = std::to_string(diffuseNr++);
		//else if (name == "texture_specular")
		//	number = std::to_string(specularNr++); // transfer unsigned int to stream
		//else if (name == "texture_normal")
		//	number = std::to_string(normalNr++); // transfer unsigned int to stream
		//else if (name == "texture_height")
		//	number = std::to_string(heightNr++); // transfer unsigned int to stream

		//printf("%f\n", (name + number).c_str());

		// now set the sampler to the correct texture unit
		glUniform1i(glGetUniformLocation(shader->ID, name.c_str()), i);
		// and finally bind the texture
		glBindTexture(GL_TEXTURE_2D, textures[i].id);
	}

	// draw mesh
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	// always good practice to set everything back to defaults once configured.
	glActiveTexture(GL_TEXTURE0);
}

void AnimatedModel::Start()
{
}

void AnimatedModel::Update()
{
	Draw();
}
