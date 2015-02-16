#include "Line.h"


Line::Line(GLuint shaderID)
{
	this->shaderID = shaderID;
}

void Line::GenerateBuffer(std::vector<glm::vec3> points)
{
	this->points = points;

	for(int i = 0; i <points.size(); i++)
	{
		color.push_back(glm::vec4(0,1,0,0));
	}

	//Initialize VAO
	glGenVertexArrays( 1, &vao );
	glBindVertexArray( vao );

	//Calc Array Sizes
	vSize = points.size() * sizeof(glm::vec3);
	cSize = color.size() * sizeof(glm::vec4);

	//Initialize VBO
	glGenBuffers( 1, &vbo );
	glBindBuffer( GL_ARRAY_BUFFER, vbo );
	glBufferData( GL_ARRAY_BUFFER, vSize + cSize, NULL, GL_STATIC_DRAW);
	glBufferSubData( GL_ARRAY_BUFFER, 0, vSize, (const GLvoid*)(&points[0]) );
	glBufferSubData( GL_ARRAY_BUFFER, vSize, cSize, (const GLvoid*)(&color[0]) );

	glEnableVertexAttribArray( 0 );
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(vSize));
	//Unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Line::SetColor(glm::vec4 col)
{
	for(unsigned int i=0; i<color.size(); ++i)
	{
		color[i] = col;
	}

	glBindVertexArray( vao );
	glBindBuffer( GL_ARRAY_BUFFER, vbo );

	glBufferSubData( GL_ARRAY_BUFFER, vSize, cSize, (const GLvoid*)(&color[0]) );

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Line::SetFromTo(glm::vec3 from, glm::vec3 to)
{
	points[0] = from;
	points[1] = to;

	glBindVertexArray( vao );
	glBindBuffer( GL_ARRAY_BUFFER, vbo );

	glBufferSubData( GL_ARRAY_BUFFER, 0, vSize, (const GLvoid*)(&points[0]));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}

void Line::DrawLine()
{
	glBindVertexArray (vao);
	glDrawArrays (GL_LINES, 0, points.size());
	glBindVertexArray(0);
}

void Line::Update(glm::mat4 view, glm::mat4 proj, float deltaTime)
{
	glm::mat4 model = glm::mat4(1.0f);

	modelLoc = glGetUniformLocation(shaderID, "model");
	viewLoc = glGetUniformLocation(shaderID, "view");
	projLoc = glGetUniformLocation(shaderID, "projection");

	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, &proj[0][0]);
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);
}
