#include "Cloth.h"


Cloth::Cloth(float width, float height, int numParticlesHeight, int numParticlesWidth)
{
	position = glm::vec3(0,12,0);
	orientation = glm::quat(glm::vec3(0,0,0));
	scale = glm::vec3(1,1,1);

	this->width = width; 
	this->height = height;

	this->numParticlesHeight = numParticlesHeight;
	this->numParticlesWidth = numParticlesWidth;
	numParticles = this->numParticlesHeight * this->numParticlesWidth;

	particles.resize(numParticles); //I am essentially using this vector as an array with room for num_particles_width*num_particles_height particles
	orderedParticles.resize(numParticles);

	// creating particles in a grid of particles from (0,0,0) to (width,-height,0)
	for(int x=0; x<numParticlesWidth; x++)
	{
		for(int y=0; y<numParticlesHeight; y++)
		{
			glm::vec3 pos = glm::vec3(width * (x/(float)numParticlesWidth),-height * (y/(float)numParticlesHeight),0);
			particles[y*numParticlesWidth+x] = Particle(pos); // insert particle in column x at y'th row
		}
	}

	// Connecting immediate neighbor particles with constraints (distance 1 and sqrt(2) in the grid)
	for(int x=0; x<numParticlesWidth; x++)
	{
		for(int y=0; y<numParticlesHeight; y++)
		{
			if (x<numParticlesWidth -1) SetConstraint(GetParticle(x,y),GetParticle(x+1,y));
			if (y<numParticlesHeight -1) SetConstraint(GetParticle(x,y),GetParticle(x,y+1));
			if (x<numParticlesWidth -1 && y<numParticlesHeight-1) SetConstraint(GetParticle(x,y),GetParticle(x+1,y+1));
			if (x<numParticlesWidth -1 && y<numParticlesHeight-1) SetConstraint(GetParticle(x+1,y),GetParticle(x,y+1));
		}
	}


	// Connecting secondary neighbors with constraints (distance 2 and sqrt(4) in the grid)
	for(int x=0; x < numParticlesWidth; x++)
	{
		for(int y=0; y < numParticlesHeight; y++)
		{
			if (x<numParticlesWidth-2) SetConstraint(GetParticle(x,y),GetParticle(x+2,y));
			if (y<numParticlesHeight-2) SetConstraint(GetParticle(x,y),GetParticle(x,y+2));
			if (x<numParticlesWidth-2 && y<numParticlesHeight-2) SetConstraint(GetParticle(x,y),GetParticle(x+2,y+2));
			if (x<numParticlesWidth-2 && y<numParticlesHeight-2) SetConstraint(GetParticle(x+2,y),GetParticle(x,y+2));			}
	}


	// making the upper left most three and right most three particles unmovable
	for(int i=0;i<3; i++)
	{
		GetParticle(0+i ,0)->OffsetPosition(glm::vec3(0.5,0.0,0.0)); // moving the particle a bit towards the center, to make it hang more natural
		//GetParticle(0+i ,0)->MakeUnmovable();

		GetParticle(0+i ,0)->OffsetPosition(glm::vec3(-0.5,0.0,0.0)); // moving the particle a bit towards the center, to make it hang more natural
		GetParticle(numParticlesWidth-i ,0)->MakeUnmovable();
	}
}

glm::vec3 Cloth::CalcNormal(Particle *p1, Particle *p2, Particle *p3)
{
	glm::vec3 v1 = p2->GetPos() - p1->GetPos();
	glm::vec3 v2 = p3->GetPos() - p1->GetPos(); 

	return glm::cross(v1,v2);
}

void Cloth::ApplyForceToTriangle(Particle *p1,Particle *p2,Particle *p3, glm::vec3 direction)
{
	glm::vec3 normal = CalcNormal(p1,p2,p3);
	glm::vec3 d = glm::normalize(normal);
	glm::vec3 force = normal * (glm::dot(d,direction));
	p1->AddForce(force);
	p2->AddForce(force);
	p3->AddForce(force);
}

void Cloth::AddForce(glm::vec3 direction)
{
	std::vector<Particle>::iterator it;

	for(it = particles.begin(); it != particles.end(); it++)
	{
		(*it).AddForce(direction); // add the forces to each particle
	}
}

void Cloth::AddWind(glm::vec3 direction)
{
	for(int x = 0; x<numParticlesWidth-2; x++)
	{
		for(int y=0; y<numParticlesHeight-2; y++)
		{
			ApplyForceToTriangle(GetParticle(x+1,y),GetParticle(x,y),GetParticle(x,y+1),direction);
			ApplyForceToTriangle(GetParticle(x+1,y+1),GetParticle(x+1,y),GetParticle(x,y+1),direction);
		}
	}
}

void Cloth::AddBallCollision(glm::vec3 centre, float radius)
{
	std::vector<Particle>::iterator it;
	for(it = particles.begin(); it != particles.end(); it++)
	{
		glm::vec3 temp = glm::vec3((*it).GetPos().x,(*it).GetPos().y + 12, (*it).GetPos().z)  - centre;
		float l = glm::length(temp);

		if (l < radius) // if the particle is inside the ball
		{
			(*it).OffsetPosition(glm::normalize(temp) * (radius-l)); // project the particle to the surface of the ball
		}
	}
}

void Cloth::AddPlaneCollision(glm::vec3 planePos)
{
	//pointOnPlane = closestPoint - (glm::dot((closestPoint - glm::vec3(0,0,0)),normal)) * normal; 
}

void Cloth::AddSelfCollision()
{
	// TO DO
}

void Cloth::TimeStep()
{
	std::vector<Constraint>::iterator constraint;

	for(int i=0; i<CONSTRAINT_ITERATIONS; i++) // iterate over all constraints several times
	{
		for(constraint = constraints.begin(); constraint != constraints.end(); constraint++ )
		{
			(*constraint).HandleContraint(); // satisfy constraint.
		}
	}

	std::vector<Particle>::iterator it;

	for(it = particles.begin(); it != particles.end(); it++)
	{
		(*it).CalculateNextPosition(); // calculate the position of each particle at the next time step.
	}
}

void Cloth::GenerateBuffer()
{
	for(int x = 0; x<numParticlesWidth-2; x++)
	{
		for(int y=0; y<numParticlesHeight-2; y++)
		{
			if(x < width / 1.5)
			{
				orderedColors.push_back((glm::vec3(0,1,0)));
				orderedColors.push_back((glm::vec3(0,1,0)));
				orderedColors.push_back((glm::vec3(0,1,0)));
			}
			else if(x > (width / 1.5) && x < (width * 1.4f))
			{
				orderedColors.push_back(glm::vec3(1,1,1));
				orderedColors.push_back(glm::vec3(1,1,1));
				orderedColors.push_back(glm::vec3(1,1,1));
			}
			else
			{
				orderedColors.push_back(glm::vec3(1,0.5f,0));
				orderedColors.push_back(glm::vec3(1,0.5f,0));
				orderedColors.push_back(glm::vec3(1,0.5f,0));
			}

			triangles.push_back(Triangle(shaderID, GetParticle(x+1,y), GetParticle(x,y),GetParticle(x,y+1)));
			triangles.push_back(Triangle(shaderID, GetParticle(x+1,y+1),GetParticle(x+1,y),GetParticle(x,y+1)));
		}
	}

	for(int i = 0; i < triangles.size(); i++)
	{

		triangles[i].SetColor(orderedColors[i]);

		//if(i % 3 == 0)
		//{
		//	triangles[i].SetColor(glm::vec3(1,1,1));
		//}
		//else if(i % 3 == 1)
		//{
		//	triangles[i].SetColor(glm::vec3(1,0.5f,0));
		//}
		//else
		//{
		//	triangles[i].SetColor(glm::vec3(1,0.5f,0));
		//}
	}
}

void Cloth::DrawCloth()
{
	for(int i = 0; i < triangles.size(); i++)
	{
		triangles[i].Draw();
	}
}

void Cloth::Update(glm::mat4 view, glm::mat4 proj)
{
	orderedParticles.clear();

	for(int x = 0; x<numParticlesWidth-2; x++)
	{
		for(int y=0; y<numParticlesHeight-2; y++)
		{
			orderedParticles.push_back(GetParticle(x+1,y));
			orderedParticles.push_back(GetParticle(x,y));
			orderedParticles.push_back(GetParticle(x,y+1));
			orderedParticles.push_back(GetParticle(x+1,y+1));
			orderedParticles.push_back(GetParticle(x+1,y));
			orderedParticles.push_back(GetParticle(x,y+1));

			//triangles.push_back(Triangle(shaderID, GetParticle(x+1,y), GetParticle(x,y),GetParticle(x,y+1)));
			//triangles.push_back(Triangle(shaderID, GetParticle(x+1,y+1),GetParticle(x+1,y),GetParticle(x,y+1)));
		}
	}

	for(int i = 0; i < triangles.size(); i++)
	{
		triangles[i].SetP1(orderedParticles[i*3]);
		triangles[i].SetP2(orderedParticles[i*3+1]);
		triangles[i].SetP3(orderedParticles[i*3+2]);

		triangles[i].Update();
	}

	glm::mat4 model = GetTransformationMatrix();

	modelLoc = glGetUniformLocation(shaderID, "model");
	viewLoc = glGetUniformLocation(shaderID, "view");
	projLoc = glGetUniformLocation(shaderID, "projection");

	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, &proj[0][0]);
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);
}

glm::mat4 Cloth::GetTransformationMatrix()
{
	glm::mat4 translationM = glm::translate(position);
	glm::mat4 rotationM = glm::toMat4(orientation);
	glm::mat4 scaleM = glm::scale(scale);

	return translationM * rotationM * scaleM;
}

float Cloth::RandomNumber(float Min, float Max)
{
	return ((float(rand()) / float(RAND_MAX)) * (Max - Min)) + Min;
}