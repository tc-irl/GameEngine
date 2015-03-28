#include "Triangle.h"


Triangle::Triangle(GLuint shaderID, Particle *p1, Particle *p2, Particle *p3, int ID)
{
	this->shaderID = shaderID;
	this->p1 = p1;
	this->p2 = p2;
	this->p3 = p3;

	this->ID = ID;

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

glm::vec3 Triangle::GetTriangleNormal()
{
	glm::vec3 v1 = p2->GetPos() - p1->GetPos();
	glm::vec3 v2 = p3->GetPos() - p1->GetPos(); 

	return glm::cross(v1,v2);
}

bool Triangle::RayIntersectsTriangle(glm::vec3 previousPos, glm::vec3 pos)
{
	glm::vec3 e1 = p2->GetPos() - p1->GetPos();
	glm::vec3 e2 = p3->GetPos() - p1->GetPos();

	glm::vec3 rayDir = pos - previousPos;

	glm::vec3 pvec = glm::cross(rayDir,e2);

	float det = glm::dot(e1, pvec);

	if(det < 0.000001 && det > 0.000001)
	{
		return false;
	}

	float invDet = 1 / det;

	glm::vec3 tvec = previousPos - p1->GetPos();

	float u = glm::dot(tvec, pvec) * invDet;

	if (u < 0 || u > 1)
		return false;
	// prepare to compute v
	glm::vec3 qvec = glm::cross(tvec, e1);
	float v = glm::dot(rayDir, qvec) * invDet;
	if (v < 0 || u + v > 1)
		return false;
	// calculate t, ray intersects triangle
	float t = glm::dot(e2, qvec) * invDet;

	return true;
	//float r,a,b;
	//glm::vec3 u = p2->GetPos() - p1->GetPos();
	//glm::vec3 v = p3->GetPos() - p1->GetPos();

	//glm::vec3 normal = glm::cross(u,v);

	//if(normal == glm::vec3(0))
	//{
	//	return -1;
	//}

	//glm::vec3 dir = pos - previousPos;
	//glm::vec3 w0 = previousPos - p1->GetPos();

	//a = -glm::dot(normal,w0);
	//b = glm::dot(normal, dir);

	//if(fabs(b) < SMALL_NUM)
	//{
	//	if(a == 0)
	//	{
	//		return 2;
	//	}
	//	else
	//	{
	//		return 0;
	//	}
	//}



	//r = a / b; 

	//if(r < 0.0)
	//{
	//	return 0;
	//}
	//else if (r > 1.0)
	//{
	//	return 0;
	//}

	//glm::vec3 intersectionPoint = previousPos + r * dir;

	//float uu, uv, vv, wu, wv, D;

	//uu = glm::dot(u, u);
	//uv = glm::dot(u, v);
	//vv = glm::dot(v, v);
	//
	//glm::vec3 w = intersectionPoint - p1->GetPreviousPos();

	//wu = glm::dot(w, u);
	//wv = glm::dot(w, v);
	//D = uv * uv - uu * vv;

	//float s, t;

	//s = (uv * wv - vv * wu) / D;
	//
	//if(s < 0.0 || s > 1.0)
	//{
	//	return 0;
	//}

	//t = (uv * wu - uu * wv) / D;

	//if(t < 0.0 || (s + t) > 1.0)
	//{
	//	return 0;
	//}

	//return 1;
}

bool Triangle::IsPointInTriangle(glm::vec3 p)
{
	//glm::vec3 a, b, c;

	//a = p1->GetPos();
	//b = p2->GetPos();
	//c = p3->GetPos();

	glm::vec3 u = p2->GetPos() - p1->GetPos();
	glm::vec3 v = p3->GetPos() - p1->GetPos();
	glm::vec3 w = p - p1->GetPos();

	glm::vec3 vCrossW = glm::cross(v, w);
	glm::vec3 vCrossU = glm::cross(v, u);

	// Test sign of r
	if (glm::dot(vCrossW, vCrossU) < 0)
		return false;

	glm::vec3 uCrossW = glm::cross(u, w);
	glm::vec3 uCrossV =  glm::cross(u, v);

	// Test sign of t
	if (glm::dot(uCrossW, uCrossV) < 0)
		return false;

	// At this point, we know that r and t and both > 0.
	// Therefore, as long as their sum is <= 1, each must be less <= 1
	float denom = glm::length(uCrossV);
	float r = glm::length(vCrossW) / denom;
	float t = glm::length(uCrossW) / denom;

	return (r + t <= 1);
}
