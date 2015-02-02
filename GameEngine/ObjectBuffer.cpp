#include "ObjectBuffer.h"

ObjectBuffer::ObjectBuffer(GLuint numVertices)
{
	this->numVertices = numVertices;
}

GLuint ObjectBuffer::GenerateVBO(glm::vec3 vertices[], glm::vec3 colors[])
{
	for(int i = 0; i < numVertices; i++)
	{
		this->vertices[i] = vertices[i]; 
		this->colors[i]  = colors[i];
	}

	glGenVertexArrays(1,&VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	// In OpenGL, we bind (make active) the handle to a target name and then execute commands on that target
	// Buffer will contain an array of vertices 
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// After binding, we now fill our object with data, everything in "Vertices" goes to the GPU
	glBufferData(GL_ARRAY_BUFFER, numVertices*6*sizeof(glm::vec3), NULL, GL_STATIC_DRAW);

	// if you have more data besides vertices (e.g., vertex colors or normals), use glBufferSubData to tell the buffer when the vertices array ends and when the colors start
	glBufferSubData (GL_ARRAY_BUFFER, 0, numVertices*sizeof(glm::vec3), this->vertices);
	glBufferSubData (GL_ARRAY_BUFFER, numVertices*sizeof(glm::vec3), numVertices*sizeof(glm::vec3), this->colors);

	return VBO;
}

GLuint ObjectBuffer::GenerateVBO(std::vector<glm::vec3> ind,glm::vec3 cols[])
{
	GLuint numIndices = ind.size() * sizeof(glm::vec3);

	for(int i = 0; i < numVertices; i++)
	{
		colors[i]  = cols[i];
	}

	for(int i = 0; i < ind.size(); i++)
	{
		indices.push_back(ind[i]);
	}

	glGenVertexArrays(1,&VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	// In OpenGL, we bind (make active) the handle to a target name and then execute commands on that target
	// Buffer will contain an array of vertices 
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// After binding, we now fill our object with data, everything in "Vertices" goes to the GPU
	glBufferData(GL_ARRAY_BUFFER, numVertices*6*sizeof(GLfloat), NULL, GL_STATIC_DRAW);

	// if you have more data besides vertices (e.g., vertex colors or normals), use glBufferSubData to tell the buffer when the vertices array ends and when the colors start
	glBufferSubData (GL_ARRAY_BUFFER, 0, numIndices, (const GLvoid*) &indices[0]);
	glBufferSubData (GL_ARRAY_BUFFER, numIndices, numVertices*3*sizeof(GLfloat), colors);

	return VBO;
}

void ObjectBuffer::LinkBufferToShader(GLuint shaderProgramID)
{
	// find the location of the variables that we will be using in the shader program
	GLuint positionID = glGetAttribLocation(shaderProgramID, "vPosition");
	GLuint colorID = glGetAttribLocation(shaderProgramID, "vColor");

	// Have to enable this
	glEnableVertexAttribArray(positionID);
	// Tell it where to find the position data in the currently active buffer (at index positionID)
	glVertexAttribPointer(positionID, 3, GL_FLOAT, GL_FALSE, 0, 0);
	// Similarly, for the color data.
	glEnableVertexAttribArray(colorID);
	glVertexAttribPointer(colorID, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(numVertices*sizeof(glm::vec3)));

	glBindVertexArray(0); // unbind vao

}

GLuint ObjectBuffer::GetVBO()
{
	return VBO;
}

GLuint ObjectBuffer::GetVAO()
{
	return VAO;
}

void ObjectBuffer::DeleteVAO()
{
	glDeleteVertexArrays(1,&VAO);
}

void ObjectBuffer::DeleteVBO()
{
	glDeleteBuffers(1,&VBO);
}


void ObjectBuffer::transVertices(glm::quat orientation,glm::vec3 position)
{
	for(int i = 0; i < indices.size(); i++)
	{
		transformedVertices[i] = glm::vec3(indices[i]) * orientation + glm::vec3(position);
	}
}


// Based on http://paulbourke.net/geometry/polygonmesh/

glm::vec3 ObjectBuffer::getCentreOfMass()
{  
	GLint numIndices = indices.size();
	glm::vec3 *average = new glm::vec3[numIndices];
	glm::vec3 *area = new glm::vec3[numIndices];
	glm::vec3 sumOfAR;
	glm::vec3 sumOfA;

	for(int i = 0; i < numIndices; i+=3)
	{
		glm::vec3 a,b,c; 

		a = indices[i];
		b = indices[i+1];
		c = indices[i+2];

		average[i] = (a + b + c) / 3.0f;
		area[i] = glm::abs(glm::cross((b - a), (c -a)));

		sumOfA += area[i];
		sumOfAR += area[i] * average[i];
	}

	centroid = sumOfAR / sumOfA;

	return centroid;
}


void ObjectBuffer::Draw()
{
	glBindVertexArray(this->VAO);
	glDrawArrays(GL_TRIANGLES, 0, numVertices);
	glBindVertexArray(0);
}

void ObjectBuffer::verticesToPoints(std::vector<glm::vec3> & points)
{
	indices.clear();

	for(unsigned int i=0; i<points.size(); ++i)
	{
		bool flag = false;

		for(unsigned int j=0; j<indices.size(); ++j)
		{
			if(points[i] == indices[j])
			{
				flag = true;
				break;
			}
		}

		if(!flag)
			indices.push_back(vertices[i]); 
	}
}


