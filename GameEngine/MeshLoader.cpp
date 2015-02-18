#include "MeshLoader.h"


MeshLoader::MeshLoader(GLuint initialShaderID, const char* filename)
{
	this->filename = filename;
	shaderID = initialShaderID;

	position = glm::vec3(0,0,0);
	orientation = glm::quat(glm::vec3(0,0,0));
	scale = glm::vec3(1,1,1);

	useTexture = true;
	useNormalTexture = true;
	dispersion = false;

	SetAttributesAndUniforms();
	LoadMesh(filename);

	reflectionFactor = 1.0f;

	refractiveIndex = AIRTODIAMOND;
	refractiveIndexR = AIRTODIAMOND;
	refractiveIndexG = AIRTODIAMOND;
	refractiveIndexB = AIRTODIAMOND;

	ratioR = 0.65;
	ratioG = 0.67;
	ratioB = 0.69;
}

MeshLoader::~MeshLoader(void)
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

void MeshLoader::LoadMesh(const char* filename)
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
		cSize = vertices.size() * sizeof(glm::vec4);

		glGenBuffers(1, &VBO[VERTEX_BUFFER]);
		glBindBuffer(GL_ARRAY_BUFFER, VBO[VERTEX_BUFFER]);
		glBufferData(GL_ARRAY_BUFFER, vSize + cSize, NULL, GL_STATIC_DRAW);
		glBufferSubData( GL_ARRAY_BUFFER, 0, vSize, (const GLvoid*)(&vertices[0]));

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray (0);

		// Just incase we use colors
		glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0,  BUFFER_OFFSET(vSize));
		glEnableVertexAttribArray (vColor);
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

void MeshLoader::SetTexture(const char* filename)
{
	textureName = filename;

	glUniform1i(gSampler, 0);

	texture = new Texture(GL_TEXTURE_2D, filename);

	if (!texture->Load()) 
	{
		std::cout << "Unable to load texture" << std::endl;
	}
}

//void MeshLoader::SetNormalTexture(const char* textureName, const char* normalName)
//{
//	this->filename = textureName;
//	this->normalTextureName = normalName;
//
//	normalTexture = new Texture(GL_TEXTURE_2D,filename,normalTextureName,gSampler, nSampler);
//
//	//if (!normalTexture->Load()) 
//	//{
//	//	std::cout << "Unable to load normal texture" << std::endl;
//	//}
//
//}


void MeshLoader::SetNormalTexture(const char* textureName)
{
	this->filename = textureName;

	glUniform1i(nSampler, 1);

	normalTexture = new Texture(GL_TEXTURE_2D,filename);

	if (!normalTexture->Load()) 
	{
		std::cout << "Unable to load normal texture" << std::endl;
	}

}

void MeshLoader::SetCubeMapTexture(const char* directory)
{
	glUniform1i(cSampler, 0);

	cubeTexture = new CubeMapTexture(GL_TEXTURE_CUBE_MAP, directory);

	if (!cubeTexture->LoadCubeMap()) 
	{
		std::cout << "Unable to load cube map" << std::endl;
	}
}



void MeshLoader::SetColor(glm::vec3 color)
{
	this->color = color;

	previousColor = color;

	std::vector<glm::vec4> newColor; 

	for(int i = 0; i < vertices.size(); i++)
	{
		newColor.push_back(glm::vec4(color,1.0f));
	}

	vSize = vertices.size() * sizeof(glm::vec3);
	cSize = newColor.size() * sizeof(glm::vec4);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[VERTEX_BUFFER]);
	glBufferSubData( GL_ARRAY_BUFFER, vSize, cSize, (const GLvoid*)(&newColor[0]));

	glBindVertexArray(0);
}

void MeshLoader::SetAttributesAndUniforms()
{
	vColor = glGetAttribLocation(shaderID, "vColor");
	gSampler = glGetUniformLocation(shaderID, "gSampler");
	cSampler = glGetUniformLocation(shaderID, "cSampler");
	nSampler = glGetUniformLocation(shaderID, "nSampler");
	modelLoc = glGetUniformLocation(shaderID, "model");
	viewLoc = glGetUniformLocation(shaderID, "view");
	projLoc = glGetUniformLocation(shaderID, "projection");
	vEye = glGetUniformLocation(shaderID, "vEye");
	camPos = glGetUniformLocation(shaderID, "cameraPosition");
	drawSky = glGetUniformLocation(shaderID, "DrawSkyBox");
	reflectFactor = glGetUniformLocation(shaderID, "reflectFactor");
	materialColor = glGetUniformLocation(shaderID, "materialColor");
	ratioID = glGetUniformLocation(shaderID, "ratio");
	normalMap = glGetUniformLocation(shaderID, "vTangent");
	ratioRID = glGetUniformLocation(shaderID, "ratioR");
	ratioGID = glGetUniformLocation(shaderID, "ratioG");
	ratioBID = glGetUniformLocation(shaderID, "ratioB");
}

glm::mat4 MeshLoader::GetTransformationMatrix()
{
	glm::mat4 translationM = glm::translate(position);
	glm::mat4 rotationM = glm::toMat4(orientation);
	glm::mat4 scaleM = glm::scale(scale);

	return translationM * rotationM * scaleM;
}

void MeshLoader::VerticesToPoints(std::vector<glm::vec3> vert)
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

void MeshLoader::Rotate360(float dt)
{
	orientation *= glm::quat(glm::vec3(0,dt,0));
}

void MeshLoader::Update(glm::mat4 view, glm::mat4 proj, float deltaTime)
{
	glm::vec3 temp = previousColor;
	glm::vec3 newColor = color;

	if(temp != newColor)
	{
		SetColor(newColor);
	}

	SetSkyBox();

	glm::mat4 model = GetTransformationMatrix();

	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, &proj[0][0]);
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);
}


void MeshLoader::Render()
{
	if(useTexture)
	{
		// Bind our diffuse texture in Texture Unit 0
		glActiveTexture(GL_TEXTURE0);
		texture->Bind(GL_TEXTURE0);
		// Set our "DiffuseTextureSampler" sampler to user Texture Unit 0
		glUniform1i(gSampler, 0);

	}
	else
	{
		texture->UnBind();
	}

	if(useNormalTexture)
	{
		glActiveTexture(GL_TEXTURE1);
		normalTexture->Bind(GL_TEXTURE1);
		// Set our "Normal	TextureSampler" sampler to user Texture Unit 0
		glUniform1i(nSampler, 1);
	}
	else
	{
		normalTexture->UnBind();
	}

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_INT, NULL);
	glBindVertexArray(0);
}

void MeshLoader::RenderCubeMap()
{
	cubeTexture->Bind(GL_TEXTURE0);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_INT, NULL);
	glBindVertexArray(0);
}

void MeshLoader::RenderPoly()
{
	glBindVertexArray(VAO);
	glDrawElements(GL_LINE_STRIP, numElements, GL_UNSIGNED_INT, NULL);
	glBindVertexArray(0);
}

void MeshLoader::UpdateShader()
{
	SetShader(possibleShaders[shaderType]);
	UseProgram();
}

void MeshLoader::UpdateRefractionIndex()
{
	SetCurrentRatio(refractions[refractiveIndex]);
}

void MeshLoader::UpdateRefractionIndexRGB()
{
	//SetCurrentRatio(refractions[refractiveIndexR],refractions[refractiveIndexG],refractions[refractiveIndexB]);
	SetCurrentRatio(ratioR,ratioG,ratioB);
}


void MeshLoader::SetSkyBox()
{
	//glUniform3f(camPos, cameraPos.x, cameraPos.y, cameraPos.z);
	//glUniform1i(drawSky, drawSkyBox);
	glUniform4f(materialColor, matColor.r, matColor.g, matColor.b, matColor.a);
	glUniform1f(reflectFactor, reflectionFactor);
}

void MeshLoader::EnableDispersion()
{
	glUniform1f(disperionID,dispersion);
}


void MeshLoader::SetCurrentRatio(float ratio)
{
	glUniform1f(ratioID,ratio);
}

void MeshLoader::SetCurrentRatio(float ratioR, float ratioG,float ratioB)
{
	glUniform1f(ratioRID,ratioR);
	glUniform1f(ratioGID,ratioG);
	glUniform1f(ratioBID,ratioB);
}