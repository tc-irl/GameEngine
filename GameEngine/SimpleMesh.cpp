#include "SimpleMesh.h"
SimpleMesh::SimpleMesh(GLuint shaderID, const char *filename, glm::vec3 color, bool useTexture)
{
	this->shaderID = shaderID;
	this->useTexture = useTexture;
	this->color = color;
	
	position = glm::vec3(0,0,0);
	orientation = glm::quat(glm::vec3(0,0,0));
	scale = glm::vec3(1,1,1);

	LoadMesh(filename);
}

SimpleMesh::SimpleMesh(GLuint shaderID, const char *filename, bool useTexture)
{
	this->useTexture = useTexture;
	this->shaderID = shaderID;

	position = glm::vec3(0,0,0);
	orientation = glm::quat(glm::vec3(0,0,0));
	scale = glm::vec3(1,1,1);

	LoadMesh(filename);
}

SimpleMesh::SimpleMesh(GLuint shaderID, const char *filename, bool useTexture, glm::vec3 position, glm::quat orientation, glm::vec3 scale)
{
	this->position = position;
	this->scale = scale;
	this->orientation = orientation;
	this->useTexture = useTexture;

	LoadMesh(filename);
}

SimpleMesh::SimpleMesh(GLuint shaderID, const char *filename, glm::vec3 color, bool useTexture, glm::vec3 position, glm::quat orientation, glm::vec3 scale)
{
	this->position = position;
	this->scale = scale;
	this->orientation = orientation;
	this->useTexture = useTexture;

	this->color = color;

	LoadMesh(filename);
}

void SimpleMesh::initTexture(const char* filename)
{
	gSampler = glGetUniformLocation(shaderID, "gSampler");

	glUniform1i(gSampler, 0);

	texture = new Texture(GL_TEXTURE_2D, filename);

	if (!texture->Load()) 
	{
		std::cout << "Unable to load texture" << std::endl;
	}
}

void SimpleMesh::update(float deltaTime)
{
	GLuint modelLoc = glGetUniformLocation(shaderID, "model");

	glm::mat4 model;

	model = getTransformationMatrix();

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);
}

void SimpleMesh::update(glm::mat4 model, float deltaTime)
{
	GLuint modelLoc = glGetUniformLocation(shaderID, "model");

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);
}


void SimpleMesh::render()
{
	if(useTexture)
	{
		texture->Bind(GL_TEXTURE0);
	}

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_INT, NULL);
	glBindVertexArray(0);
}

void SimpleMesh::renderPoly()
{
	glBindVertexArray(VAO);
	glDrawElements(GL_LINE_STRIP, numElements, GL_UNSIGNED_INT, NULL);
	glBindVertexArray(0);
}

glm::mat4 SimpleMesh::getTransformationMatrix()
{
	glm::mat4 translationM = glm::translate(position);
	glm::mat4 rotationM = glm::toMat4(orientation);
	glm::mat4 scaleM = glm::scale(scale);

	return translationM * rotationM * scaleM;
}

void SimpleMesh::LoadMesh(const char *filename)
{
	Assimp::Importer importer;
	aiMesh *mesh;

	const aiScene *scene = importer.ReadFile(filename, 	aiProcess_Triangulate
		| aiProcess_OptimizeGraph
		| aiProcess_OptimizeMeshes
		| aiProcess_RemoveRedundantMaterials
		| aiProcess_GenSmoothNormals);

	if(!scene) 
	{
		printf("Unable to load mesh: %s\n", importer.GetErrorString());
	}

	mesh = scene->mMeshes[0];

	VBO [VERTEX_BUFFER] = NULL;
	VBO [TEXCOORD_BUFFER] = NULL;
	VBO [NORMAL_BUFFER] = NULL;
	VBO [INDEX_BUFFER] = NULL;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	numElements = mesh->mNumFaces * 3;

	if(mesh->HasPositions() && useTexture == true)
	{
		for(int i = 0; i < mesh->mNumVertices; i++) 
		{
			vertices.push_back(glm::vec3(mesh->mVertices[i].x,mesh->mVertices[i].y,mesh->mVertices[i].z));
		}

		verticesToPoints(vertices);

		glGenBuffers(1, &VBO[VERTEX_BUFFER]);
		glBindBuffer(GL_ARRAY_BUFFER, VBO[VERTEX_BUFFER]);
		glBufferData(GL_ARRAY_BUFFER, 3 * mesh->mNumVertices * sizeof(GLfloat), (const GLvoid*) &vertices[0], GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray (0);
		
	}
	else if (mesh->HasPositions() && useTexture == false)
	{

		for(int i = 0; i < mesh->mNumVertices; i++) 
		{
			vertices.push_back(glm::vec3(mesh->mVertices[i].x,mesh->mVertices[i].y,mesh->mVertices[i].z));
		}
		
		verticesToPoints(vertices);

		for(int i = 0; i < mesh->mNumVertices; i++) 
		{
			colors.push_back(glm::vec4(color.r,color.g,color.b,1.0f));
		}
		
		vSize = vertices.size() * sizeof(glm::vec3);
		cSize = colors.size() * sizeof(glm::vec4);

		glGenBuffers(1, &VBO[VERTEX_BUFFER]);
		glBindBuffer(GL_ARRAY_BUFFER, VBO[VERTEX_BUFFER]);
		glBufferData(GL_ARRAY_BUFFER, vSize + cSize, NULL, GL_STATIC_DRAW);
		glBufferSubData( GL_ARRAY_BUFFER, 0, vSize, (const GLvoid*)(&vertices[0]));
		glBufferSubData( GL_ARRAY_BUFFER, vSize, cSize, (const GLvoid*)(&colors[0]));

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray (0);
		
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0,  BUFFER_OFFSET(vSize));
		glEnableVertexAttribArray (1);
		
	}
	
	if(mesh->HasTextureCoords(0) && useTexture == true)
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

SimpleMesh::~SimpleMesh()
{
	if(VBO[VERTEX_BUFFER]) 
	{
		glDeleteBuffers(1, &VBO[VERTEX_BUFFER]);
	}

	if(VBO[TEXCOORD_BUFFER])
	{
		glDeleteBuffers(1, &VBO[TEXCOORD_BUFFER]);
	}

	if(VBO[NORMAL_BUFFER]) 
	{
		glDeleteBuffers(1, &VBO[NORMAL_BUFFER]);
	}

	if(VBO[INDEX_BUFFER]) 
	{
		glDeleteBuffers(1, &VBO[INDEX_BUFFER]);
	}

	glDeleteVertexArrays(1, &VAO);
}

void SimpleMesh::verticesToPoints(std::vector<glm::vec3> vert)
{
	points.clear();

	for(unsigned int i=0; i<vert.size(); ++i)
	{
		bool flag = false;

		for(unsigned int j=0; j<points.size(); ++j)
		{
			if(vert[i] == points[j])
			{
				flag = true;
				break;
			}
		}

		if(!flag)
			points.push_back(vert[i]); 
	}
}


void SimpleMesh::updateColor(glm::vec3 color)
{
	std::vector<glm::vec4> newColor; 

	for(int i = 0; i < vertices.size(); i++)
	{
		newColor.push_back(glm::vec4(color,1.0f));
	}

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[VERTEX_BUFFER]);
	glBufferSubData(GL_ARRAY_BUFFER, vSize, cSize, (const GLvoid*)(&newColor[0]));
	glBindVertexArray(0);

}

