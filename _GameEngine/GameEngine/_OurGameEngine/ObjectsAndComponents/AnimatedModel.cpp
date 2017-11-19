#include "AnimatedModel.hpp"

//fmod music library

//radias probs 

//yahiazakaria13@gmail.com

AnimatedModel::AnimatedModel(GameObject* contObject, const char* path, Shader* _shader) :Component(contObject) {

	shader = _shader;

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
	// check for errors
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
	{
		std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
		return;
	}
	//retrieve the directory path of the filepath
	//directory = path.substr(0, path.find_last_of('/'));

	shader_jointIndId = glGetAttribLocation(shader->ID, "jointInd");
	shader_weightsId = glGetAttribLocation(shader->ID, "weights");
	shader_vertexPosModelSpaceID = glGetAttribLocation(shader->ID, "vertexPosition_modelspace");

	bonesNumber = 0;

	if (scene->mMeshes[0]->HasBones())
		processMesh(scene->mMeshes[0]->mBones, scene->mMeshes[0], scene->mMeshes[0]->mNumBones);
	else
		processMesh(scene->mMeshes[0]);

	processBones(scene->mMeshes[0]->mBones, scene->mAnimations, scene->mMeshes[0]->mNumBones);

	if (scene->HasAnimations())
		processAnimation(scene->mNumAnimations, scene->mAnimations);
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
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	// vertex texture coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
	// vertex tangent
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
	// vertex bitangent
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));

	// Weights bitangent
	glEnableVertexAttribArray(shader_jointIndId);
	glVertexAttribPointer(shader_jointIndId, 2, GL_INT, GL_FALSE, sizeof(float), (void*)offsetof(Vertex, Bitangent));

	// Indexes bitangent
	glEnableVertexAttribArray(shader_weightsId);
	glVertexAttribPointer(shader_weightsId, 2, GL_FLOAT, GL_FALSE, sizeof(float), (void*)offsetof(Vertex, Bitangent));

	glBindVertexArray(0);
}

void AnimatedModel::processMesh(aiBone ** bones, aiMesh *mesh, int numBones)
{
	bonesNumber = numBones;
	BonesTransforms = new glm::mat4[numBones];

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
		glm::vec2 weights = glm::vec2(-1, -1);
		glm::ivec2 ids = glm::ivec2(-1, -1);

		for (int j = 0; j < numBones; j++)
		{
			for (int k = 0; k < bones[j]->mNumWeights; k++)
			{
				if (bones[j]->mWeights[k].mVertexId == i && numIds < 2) {
					weights[numIds] = bones[j]->mWeights[k].mWeight;
					ids[numIds] = j;

					numIds++;
				}
			}
		}

		vertex.boneWeights = weights;
		vertex.affectingBonesId = ids;


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
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	// vertex texture coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
	// vertex tangent
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
	// vertex bitangent
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));

	// Weights bitangent
	glEnableVertexAttribArray(shader_jointIndId);
	glVertexAttribPointer(shader_jointIndId, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, affectingBonesId));

	// Indexes bitangent
	glEnableVertexAttribArray(shader_weightsId);
	glVertexAttribPointer(shader_weightsId, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, boneWeights));

	glBindVertexArray(0);
}

void AnimatedModel::processBones(aiBone ** bones, aiAnimation** animations, int numBones)
{

	for (int i = 0; i < numBones; i++) {
		glm::mat4 boneTrans;

		// Interpolate scaling and generate scaling transformation matrix
		glm::vec3 Scaling = glm::vec3(animations[0]->mChannels[i + 1]->mScalingKeys[0].mValue.x, animations[0]->mChannels[0]->mScalingKeys[0].mValue.y, animations[0]->mChannels[0]->mScalingKeys[0].mValue.z);
		glm::mat4 ScalingMatrix = glm::scale(Scaling);

		aiQuaternion rotationQuat = animations[0]->mChannels[i + 1]->mRotationKeys[1].mValue;
		aiVector3D xRotated = rotationQuat.Rotate(aiVector3D(1, 0, 0));
		aiVector3D yRotated = rotationQuat.Rotate(aiVector3D(0, 1, 0));
		aiVector3D zRotated = rotationQuat.Rotate(aiVector3D(0, 0, 1));

		glm::vec3 Translation = glm::vec3(animations[0]->mChannels[i + 1]->mPositionKeys[1].mValue.x, animations[0]->mChannels[i + 1]->mPositionKeys[1].mValue.y, animations[0]->mChannels[i + 1]->mPositionKeys[1].mValue.z);
		glm::mat4 TranslationMatrix = glm::translate(Translation);

		glm::mat4 rotationMatrix = glm::translate(glm::mat4(1.0f), Translation);
		float RotaionX = acos(yRotated.y);
		float RotaionY = acos(yRotated.z);
		float RotaionZ = acos(yRotated.x);
		rotationMatrix = glm::rotate(rotationMatrix, RotaionX, glm::vec3(1, 0, 0));
		rotationMatrix = glm::rotate(rotationMatrix, RotaionY, glm::vec3(0, 1, 0));
		rotationMatrix = glm::rotate(rotationMatrix, RotaionZ, glm::vec3(0, 0, 1));

		BonesTransforms[i] = rotationMatrix * TranslationMatrix;

		//CalcInterpolatedScaling(Scaling, AnimationTime, pNodeAnim);
		//Matrix4f ScalingM = new Matrix4f().InitScale(Scaling.x(), Scaling.y(), Scaling.z());

		//// Interpolate rotation and generate rotation transformation matrix
		//Quaternion RotationQ = new Quaternion(0, 0, 0, 0);
		//CalcInterpolatedRotation(RotationQ, AnimationTime, pNodeAnim);
		//Matrix4f RotationM = RotationQ.ToRotationMatrix();

		//// Interpolate translation and generate translation transformation matrix
		//Vector3f Translation = new Vector3f(0, 0, 0);
		//CalcInterpolatedPosition(Translation, AnimationTime, pNodeAnim);
		//Matrix4f TranslationM = new Matrix4f().InitTranslation(Translation.x(), Translation.y(), Translation.z());

		//// Combine the above transformations
		//NodeTransformation = TranslationM.Mul(RotationM).Mul(ScalingM);

		aiMatrix4x4 boneTransformationToOrigin = bones[i]->mOffsetMatrix.Inverse();
		aiMatrix4x4 boneTransformationToBone = bones[i]->mOffsetMatrix;

		//aiMatrix4x4  a = animations[0]->mChannels[0]->mRotationKeys->mValue.GetMatrix();


	}
}


void AnimatedModel::Draw()
{
	shader->use();

	glm::mat4 transform = containingObject->getTransformMatrix();

	// view/projection transformations
	shader->setMat4("transform", transform);
	shader->setMat4("projection", (*universalViewProj));


	shader->setVec4("tests[0]", glm::vec4(sin(glfwGetTime()), 0, 0, 0));
	shader->setVec4("tests[1]", glm::vec4(0, cos(glfwGetTime()), 0, 0));

	shader->setInt("numBones", 5);

	for (int i = 0; i < bonesNumber; i++)
	{
		shader->setMat4("joints[" + std::to_string(i) + "]", BonesTransforms[i] /* glm::mat4()*/);
	}



	// bind appropriate textures
	//unsigned int diffuseNr = 1;
	//unsigned int specularNr = 1;
	//unsigned int normalNr = 1;
	//unsigned int heightNr = 1;
	//for (unsigned int i = 0; i < textures.size(); i++)
	//{
	//	glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
	//									  // retrieve texture number (the N in diffuse_textureN)
	//	std::string number;
	//	std::string name = textures[i].type;
	//	if (name == "texture_diffuse")
	//		number = std::to_string(diffuseNr++);
	//	else if (name == "texture_specular")
	//		number = std::to_string(specularNr++); // transfer unsigned int to stream
	//	else if (name == "texture_normal")
	//		number = std::to_string(normalNr++); // transfer unsigned int to stream
	//	else if (name == "texture_height")
	//		number = std::to_string(heightNr++); // transfer unsigned int to stream

	//											 // now set the sampler to the correct texture unit
	//	glUniform1i(glGetUniformLocation(shader->ID, (name + number).c_str()), i);
	//	// and finally bind the texture
	//	glBindTexture(GL_TEXTURE_2D, textures[i].id);
	//}

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
