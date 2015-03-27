#include "Triangle.h"


Triangle::Triangle(GLuint shaderID, Particle *p1, Particle *p2, Particle *p3)
{
	this->shaderID = shaderID;
	this->p1 = p1;
	this->p2 = p2;
	this->p3 = p3;

	GenerateBuffer();

	drawTriangle = true;
}

void Triangle::GenerateBuffer()
{
	points.clear();

	normals.push_back(p1->GetNormal());
	normals.push_back(p2->GetNormal());
	normals.push_back(p3->GetNormal());

	points.push_back(p1->GetPos());
	points.push_back(p2->GetPos());
	points.push_back(p3->GetPos());

	colors.push_back(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	colors.push_back(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	colors.push_back(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));

	//Initialize VAO
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	//Calc Array Sizes
	vSize = points.size() * sizeof(glm::vec3);
	cSize = colors.size() * sizeof(glm::vec4);
	nSize = normals.size() * sizeof(glm::vec3);

	//Initialize VBO
	glGenBuffers( 1, &vbo );
	glBindBuffer( GL_ARRAY_BUFFER, vbo );
	glBufferData( GL_ARRAY_BUFFER, vSize + cSize + nSize, NULL, GL_STATIC_DRAW);
	glBufferSubData( GL_ARRAY_BUFFER, 0, vSize, (const GLvoid*)(&points[0]));
	glBufferSubData( GL_ARRAY_BUFFER, vSize, cSize, (const GLvoid*)(&colors[0]));
	glBufferSubData( GL_ARRAY_BUFFER, vSize + cSize, nSize, (const GLvoid*)(&normals[0]));

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(vSize));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(vSize + cSize));
	//Unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Triangle::SetColor(glm::vec3 color)
{
	this->color = color;

	std::vector<glm::vec4> newColor; 

	for(int i = 0; i < points.size(); i++)
	{
		newColor.push_back(glm::vec4(color,1.0f));
	}

	vSize = points.size() * sizeof(glm::vec3);
	cSize = newColor.size() * sizeof(glm::vec4);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferSubData( GL_ARRAY_BUFFER, vSize, cSize, (const GLvoid*)(&newColor[0]));

	glBindVertexArray(0);
}

void Triangle::Draw()
{
	if(drawTriangle)
	{
		glBindVertexArray (vao);
		glDrawArrays (GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);
	}
}

void Triangle::Update()
{
	points.clear();
	normals.clear();

	points.push_back(p1->GetPos());
	points.push_back(p2->GetPos());
	points.push_back(p3->GetPos());

	normals.push_back(p1->GetNormal());
	normals.push_back(p2->GetNormal());
	normals.push_back(p3->GetNormal());

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferSubData( GL_ARRAY_BUFFER, 0, vSize, (const GLvoid*)(&points[0]));
	glBufferSubData( GL_ARRAY_BUFFER, vSize + cSize, nSize, (const GLvoid*)(&normals[0]));
	glBindVertexArray(0);
}

bool Triangle::CompareParticles(Particle *p)
{
	if(p == p1)
	{
		return true;
	}
	else
	{
		return false;
	}
}
