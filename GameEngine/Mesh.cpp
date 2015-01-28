#include "Mesh.h"


Mesh::MeshEntry::MeshEntry(aiMesh *mesh)
{
	VBO [VERTEX_BUFFER] = NULL;
	VBO [TEXCOORD_BUFFER] = NULL;
	VBO [NORMAL_BUFFER] = NULL;
	VBO [INDEX_BUFFER] = NULL;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	numElements = mesh->mNumFaces * 3;

	if(mesh->HasPositions())
	{
		float *vertices = new float[mesh->mNumVertices * 3];

		for(int i = 0; i < mesh->mNumVertices; i++)
		{
			vertices[i * 3] = mesh->mVertices[i].x;
			vertices[i * 3 + 1] = mesh->mVertices[i].y;
			vertices[i * 3 + 2] = mesh->mVertices[i].z;
		}

		glGenBuffers(1,&VBO [VERTEX_BUFFER]);
		glBindBuffer(GL_ARRAY_BUFFER, VBO [VERTEX_BUFFER]);
		glBufferData(GL_ARRAY_BUFFER, mesh->mNumVertices * 3 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray (0);

		delete vertices;
	}

	if(mesh->HasTextureCoords(0))
	{
		float *texcoords = new float[mesh->mNumVertices * 2];

		for(int i = 0; i < mesh->mNumVertices; i++)
		{
			texcoords[i * 2] = mesh->mTextureCoords[0][i].x;
			texcoords[i * 2 + 1] = mesh->mTextureCoords[0][i].y;
		}

		glGenBuffers(1, &VBO [TEXCOORD_BUFFER]);
		glBindBuffer(GL_ARRAY_BUFFER, VBO [TEXCOORD_BUFFER]);
		glBufferData(GL_ARRAY_BUFFER, mesh->mNumVertices * 2 * sizeof(GLfloat), texcoords, GL_STATIC_DRAW);

		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray (1);

		delete texcoords;
	}

	if(mesh->HasNormals())
	{
		float *normals = new float[mesh->mNumVertices * 3];

		for(int i = 0; i < mesh->mNumVertices; i++)
		{
			normals[i * 3] = mesh->mNormals[i].x;
			normals[i * 3 + 1] = mesh->mNormals[i].y;
			normals[i * 3 + 2] = mesh->mNormals[i].z;
		}

		glGenBuffers(1,&VBO [NORMAL_BUFFER]);
		glBindBuffer(GL_ARRAY_BUFFER, VBO [NORMAL_BUFFER]);
		glBufferData(GL_ARRAY_BUFFER, mesh->mNumVertices * 3 * sizeof(GLfloat), normals, GL_STATIC_DRAW);

		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray (2);

		delete normals;
	}

	if(mesh->HasFaces())
	{
		GLuint *indices = new GLuint[mesh->mNumFaces * 3];

		for(int i = 0; i < mesh->mNumFaces; i++)
		{
			indices[i * 3] = mesh->mFaces[i].mIndices[0];
			indices[i * 3 + 1] = mesh->mFaces[i].mIndices[1];
			indices[i * 3 + 2] = mesh->mFaces[i].mIndices[2];
		}

		glGenBuffers(1, &VBO [INDEX_BUFFER]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBO [INDEX_BUFFER]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->mNumFaces * 3 * sizeof(GLuint), indices, GL_STATIC_DRAW);

		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray (3);

		delete indices;
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}

Mesh::MeshEntry::~MeshEntry()
{
	if(VBO[VERTEX_BUFFER]) {
		glDeleteBuffers(1, &VBO[VERTEX_BUFFER]);
	}

	if(VBO[TEXCOORD_BUFFER]) {
		glDeleteBuffers(1, &VBO[TEXCOORD_BUFFER]);
	}

	if(VBO[NORMAL_BUFFER]) {
		glDeleteBuffers(1, &VBO[NORMAL_BUFFER]);
	}

	if(VBO[INDEX_BUFFER]) {
		glDeleteBuffers(1, &VBO[INDEX_BUFFER]);
	}

	glDeleteVertexArrays(1, &VAO);
}

void Mesh::MeshEntry::render()
{
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_INT, NULL);
	glBindVertexArray(0);
}

Mesh::Mesh(const char *filename, bool useTexture)
{
	this->useTexture = useTexture;

	position = glm::vec3(0,0,0);
	orientation = glm::quat(glm::vec3(0,0,0));
	scale = glm::vec3(1,1,1);

	Assimp::Importer importer;

	const aiScene *scene = importer.ReadFile(filename, NULL);

	if(!scene) 
	{
		printf("Unable to load mesh: %s\n", importer.GetErrorString());
	}

	for(int i = 0; i < scene->mNumMeshes; ++i) 
	{
		meshEntries.push_back(new Mesh::MeshEntry(scene->mMeshes[i]));
	}
}

Mesh::Mesh(const char *filename, bool useTexture, glm::vec3 position, glm::quat orientation, glm::vec3 scale)
{
	this->position = position;
	this->scale = scale;
	this->orientation = orientation;
	this->useTexture = useTexture;

	Assimp::Importer importer;

	const aiScene *scene = importer.ReadFile(filename, NULL);

	if(!scene) {
		printf("Unable to load mesh: %s\n", importer.GetErrorString());
	}

	for(int i = 0; i < scene->mNumMeshes; ++i) 
	{
		meshEntries.push_back(new Mesh::MeshEntry(scene->mMeshes[i]));
	}
}


Mesh::~Mesh(void)
{
	for(int i = 0; i < meshEntries.size(); ++i) 
	{
		delete meshEntries.at(i);
	}

	meshEntries.clear();
}

void Mesh::initTexture(const std::string &filename, GLuint shaderID)
{
	gSampler = glGetUniformLocation(shaderID, "gSampler");

	glUniform1i(gSampler, 0);

	texture = new Texture(GL_TEXTURE_2D, filename);

	if (!texture->Load()) 
	{
		std::cout << "Unable to load texture" << std::endl;
	}
}

void Mesh::update(GLuint modelLoc, float deltaTime)
{
	glm::mat4 model;

	model = getTransformationMatrix();

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);
}


void Mesh::render()
{
	if(useTexture)
	{
		texture->Bind(GL_TEXTURE0);
	}

	for(int i = 0; i < meshEntries.size(); i++)
	{
		meshEntries[i]->render();
	}
}

glm::mat4 Mesh::getTransformationMatrix()
{
	glm::mat4 translationM = glm::translate(position);
	glm::mat4 rotationM = glm::toMat4(orientation);
	glm::mat4 scaleM = glm::scale(scale);

	return translationM * rotationM * scaleM;
}





