#include "Skybox.h"


Skybox::Skybox(GLuint initialShaderID, const char* filename)
{
	this->filename = filename;
	shaderID = initialShaderID;

	position = glm::vec3(0,0,0);
	orientation = glm::quat(glm::vec3(0,0,0));
	scale = glm::vec3(1,1,1);

	SetAttributesAndUniforms();
	LoadMesh(filename);

	reflectionFactor = 1.0f;
}

Skybox::~Skybox(void)
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

void Skybox::LoadMesh(const char* filename)
{
	Assimp::Importer importer;
	aiMesh *mesh;

	const aiScene *scene = importer.ReadFile(filename, 	aiProcess_Triangulate
		| aiProcess_OptimizeGraph
		| aiProcess_OptimizeMeshes
		| aiProcess_RemoveRedundantMaterials
		| aiProcess_GenSmoothNormals
		| aiProcess_FlipUVs
		| aiProcess_CalcTangentSpace);

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

	if(mesh->HasPositions())
	{
		for(int i = 0; i < mesh->mNumVertices; i++) 
		{
			vertices.push_back(glm::vec3(mesh->mVertices[i].x,mesh->mVertices[i].y,mesh->mVertices[i].z));
		}

		VerticesToPoints(vertices);

		vSize = vertices.size() * sizeof(glm::vec3);

		glGenBuffers(1, &VBO[VERTEX_BUFFER]);
		glBindBuffer(GL_ARRAY_BUFFER, VBO[VERTEX_BUFFER]);
		glBufferData(GL_ARRAY_BUFFER, vSize, NULL, GL_STATIC_DRAW);
		glBufferSubData( GL_ARRAY_BUFFER, 0, vSize, (const GLvoid*)(&vertices[0]));

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray (0);
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

	if(mesh->HasTangentsAndBitangents())
	{
		float *tangents = new float[mesh->mNumVertices * 3];

		for(int i = 0; i < mesh->mNumVertices; i++)
		{
			tangents[i * 3] = mesh->mTangents[i].x;
			tangents[i * 3 + 1] = mesh->mTangents[i].y;
			tangents[i * 3 + 2] = mesh->mTangents[i].z;
		}

		glGenBuffers(1,&VBO [NORMAL_MAP_BUFFER]);
		glBindBuffer(GL_ARRAY_BUFFER, VBO [NORMAL_MAP_BUFFER]);
		glBufferData(GL_ARRAY_BUFFER, mesh->mNumVertices * 3 * sizeof(GLfloat), tangents, GL_STATIC_DRAW);

		glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray (5);

		delete tangents;
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}


void Skybox::SetCubeMapTexture(const char* directory)
{
	glUniform1i(cSampler, 0);

	cubeTexture = new CubeMapTexture(GL_TEXTURE_CUBE_MAP, directory);

	if (!cubeTexture->LoadCubeMap()) 
	{
		std::cout << "Unable to load cube map" << std::endl;
	}
}

void Skybox::SetAttributesAndUniforms()
{
	cSampler = glGetUniformLocation(shaderID, "cSampler");
	modelLoc = glGetUniformLocation(shaderID, "model");
	viewLoc = glGetUniformLocation(shaderID, "view");
	projLoc = glGetUniformLocation(shaderID, "projection");
}

glm::mat4 Skybox::GetTransformationMatrix()
{
	glm::mat4 translationM = glm::translate(position);
	glm::mat4 rotationM = glm::toMat4(orientation);
	glm::mat4 scaleM = glm::scale(scale);

	return translationM * rotationM * scaleM;
}

void Skybox::VerticesToPoints(std::vector<glm::vec3> vert)
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

void Skybox::Rotate360(float dt)
{
	orientation *= glm::quat(glm::vec3(0,dt,0));
}

void Skybox::Update(glm::mat4 view, glm::mat4 proj, float deltaTime)
{
	SetSkyBox();

	glm::mat4 model = GetTransformationMatrix();

	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, &proj[0][0]);
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);
}

void Skybox::RenderCubeMap()
{
	cubeTexture->Bind(GL_TEXTURE0);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_INT, NULL);
	glBindVertexArray(0);
}

void Skybox::UpdateShader()
{
	SetShader(possibleShaders[shaderType]);
	UseProgram();
}

void Skybox::SetSkyBox()
{
	//glUniform3f(camPos, cameraPos.x, cameraPos.y, cameraPos.z);
	//glUniform1i(drawSky, drawSkyBox);
	glUniform4f(materialColor, matColor.r, matColor.g, matColor.b, matColor.a);
	glUniform1f(reflectFactor, reflectionFactor);
}
