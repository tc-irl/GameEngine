#include "Cloth.h"


Cloth::Cloth(float width, float height, int numParticlesHeight, int numParticlesWidth)
{
	position = glm::vec3(0,0,0);
	orientation = glm::quat(glm::vec3(0,0,0));
	scale = glm::vec3(1,1,1);

	this->width = width; 
	this->height = height;

	this->numParticlesHeight = numParticlesHeight;
	this->numParticlesWidth = numParticlesWidth;
	numParticles = this->numParticlesHeight * this->numParticlesWidth;

	particles.resize(numParticles); //I am essentially using this vector as an array with room for num_particles_width*num_particles_height particles
	orderedParticles.resize(numParticles);

	movable = false;

	forceEnabled = true;
	windEnabled = false;
	point1 = false;
	point2 = false;

	reset = false;

	selfCollisionEnabled = false;

	int i = 0;
	// creating particles in a grid of particles from (0,0,0) to (width,-height,0)
	for(int x=0; x<numParticlesWidth; x++)
	{
		for(int y=0; y<numParticlesHeight; y++)
		{
			
			glm::vec3 pos = glm::vec3(width * (x/(float)numParticlesWidth),-height * (y/(float)numParticlesHeight),0);
			particles[y*numParticlesWidth+x] = Particle(pos, i); // insert particle in column x at y'th row
			i++;
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
			if (x<numParticlesWidth-2 && y<numParticlesHeight-2) SetConstraint(GetParticle(x+2,y),GetParticle(x,y+2));			
		}
	}


	for(int i=0;i<3; i++)
	{
		GetParticle(0+i ,0)->OffsetPosition(glm::vec3(0.5,0.0,0.0)); // moving the particle a bit towards the center, to make it hang more natural
		GetParticle(0+i ,0)->MakeUnmovable();

		GetParticle(numParticlesWidth-i-1 ,0)->OffsetPosition(glm::vec3(-0.5,0.0,0.0)); // moving the particle a bit towards the center, to make it hang more natural
		GetParticle(numParticlesWidth-i-1 ,0)->MakeUnmovable();
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

	if(forceEnabled)
	{
		for(it = particles.begin(); it != particles.end(); it++)
		{
			(*it).AddForce(direction); // add the forces to each particle
		}
	}
}

void Cloth::AddWind(glm::vec3 direction)
{
	if(windEnabled)
	{
		for(int x = 0; x<numParticlesWidth-1; x++)
		{
			for(int y=0; y<numParticlesHeight-1; y++)
			{
				ApplyForceToTriangle(GetParticle(x+1,y),GetParticle(x,y),GetParticle(x,y+1),direction);
				ApplyForceToTriangle(GetParticle(x+1,y+1),GetParticle(x+1,y),GetParticle(x,y+1),direction);
			}
		}
	}
}

void Cloth::AddBallCollision(glm::vec3 centre, float radius)
{
	std::vector<Particle>::iterator it;
	for(it = particles.begin(); it != particles.end(); it++)
	{
		glm::vec3 temp = (glm::vec3((*it).GetPos().x,(*it).GetPos().y, (*it).GetPos().z) * orientation * scale + position)  - centre;
		float l = glm::length(temp);

		if (l < radius) // if the particle is inside the ball
		{
			(*it).OffsetPosition(glm::normalize(temp) * (radius-l)); // project the particle to the surface of the ball
			//(*it).SetFixed(true);
		}
	}
}

void Cloth::AddPlaneCollision(glm::vec3 planePos)
{
	//std::vector<Particle>::iterator it;
	//for(it = particles.begin(); it != particles.end(); it++)
	//{
	//	glm::vec3 temp = (glm::vec3((*it).GetPos().x,(*it).GetPos().y, (*it).GetPos().z) * orientation * scale + position);

	//	if (temp.y <= 0.1) // if the particle is inside the ball
	//	{
	//		
	//		//glm::vec3 nextPos = (*it).GetNextPosition();
	//		(*it).MakeUnmovable(); // project the particle to the surface of the ball
	//	}
	//}

	std::vector<Particle>::iterator it;
	for(it = particles.begin(); it != particles.end(); it++)
	{
		glm::vec3 temp = (glm::vec3((*it).GetPos().x,(*it).GetPos().y, (*it).GetPos().z) * orientation * scale + position);

		if (temp.y <= planePos.y) // if the particle is inside the ball
		{
			(*it).position.y = planePos.y + 0.01f;
		}
	}
}

void Cloth::AddTearing(glm::vec3 centre, float radius)
{
	std::vector<Particle>::iterator it;

	std::vector<Particle> hitParticles;
	std::vector<Particle>::iterator hit;
	std::vector<Constraint>::iterator constraint;
	std::vector<Triangle>::iterator triangle;
	
	for(it = particles.begin(); it != particles.end(); it++)
	{
		glm::vec3 temp = (glm::vec3((*it).GetPos().x,(*it).GetPos().y, (*it).GetPos().z) * orientation * scale + position)  - centre;

		float l = glm::length(temp);

		if (l < radius) // if the particle is inside the ball
		{
			//particles.erase(it);
			hitParticles.push_back((*it));

			for(constraint = constraints.begin(); constraint != constraints.end(); constraint++)
			{
				if(constraint->p1->GetID() == (*it).GetID() || constraint->p2->GetID() == (*it).GetID())
				{
					constraints.erase(constraint);
				}
			}

			for(triangle = triangles.begin(); triangle != triangles.end(); triangle++)
			{
				
				if(triangle->p1->GetID() == (*it).GetID() || triangle->p2->GetID() == (*it).GetID() || triangle->p3->GetID() == (*it).GetID())
				{
					triangles[triangle - triangles.begin()].drawTriangle = false;

				}
			}
		}
	}
}

void Cloth::CalculateCollisions(Particle *cP, Particle *p1,Particle *p2,Particle *p3)
{
	glm::vec3 normal = CalcNormal(p1,p2,p3);
	float length = glm::length(normal);
	glm::vec3 temp = cP->GetPos() - p1->GetPos();

	float result = glm::dot(temp, (normal / length) - 1.0f);
}

void Cloth::AddSelfCollision()
{
	if(selfCollisionEnabled)
	{
		std::vector<Particle>::iterator it;
		std::vector<Triangle>::iterator triangle;
		std::vector<Triangle>::iterator triangle2;

		//for(it = particles.begin(); it != particles.end(); it++)
		//{
		//	for(triangle = triangles.begin(); triangle != triangles.end(); triangle++)
		//	{
		//		/*		if(triangle->p1->GetID() != (*it).GetID() || triangle->p2->GetID() != (*it).GetID() || triangle->p3->GetID() != (*it).GetID())
		//		{*/
		//			bool val = triangle->RayIntersectsTriangle((*it).GetPreviousPos(),(*it).GetPos());

		//			if(val == true)
		//			{
		//				//std::cout << "Collision Detected" << std::endl;
		//				//(*it).position = (*it).GetPreviousPos();

		//				triangle->SetColor(glm::vec3(1,0,0));
		//			}
		//			else
		//			{
		//				triangle->SetColor(glm::vec3(0,0,1));
		//			}

		//		/*}*/
		//	}


		for(triangle = triangles.begin(); triangle != triangles.end(); triangle++)
		{
			for(triangle2 = triangles.begin(); triangle2 != triangles.end(); triangle2++)
			{
				if(triangle->p1->GetID() != triangle2->p1->GetID() && triangle->p1->GetID() != triangle2->p2->GetID() && triangle->p1->GetID() != triangle2->p3->GetID() 
					&& triangle->p2->GetID() != triangle2->p1->GetID() && triangle->p2->GetID() != triangle2->p2->GetID() && triangle->p2->GetID() != triangle2->p3->GetID() 
					&& triangle->p3->GetID() != triangle2->p1->GetID() && triangle->p3->GetID() != triangle2->p2->GetID() && triangle->p3->GetID() != triangle2->p3->GetID())
				{
					if(checkNarrowPhaseCollision((*triangle), (*triangle2)))
					{
						//triangle2->p1->position = triangle2->p1->GetPreviousPos();
						//triangle2->p2->position = triangle2->p2->GetPreviousPos();
						//triangle2->p3->position = triangle2->p3->GetPreviousPos();
						std::cout << "Intersection" << std::endl;

					}
				}
			}
		}
	}

}

void Cloth::TimeStep()
{
	std::vector<Particle>::iterator it;

	for(it = particles.begin(); it != particles.end(); it++)
	{
		(*it).CalculateNextPosition(position, orientation, scale); // calculate the position of each particle at the next time step.
	}

	std::vector<Constraint>::iterator constraint;

	for(int i=0; i<CONSTRAINT_ITERATIONS; i++) // iterate over all constraints several times
	{
		for(constraint = constraints.begin(); constraint != constraints.end(); constraint++ )
		{
			(*constraint).HandleContraint(); // satisfy constraint.
		}
	}
}

void Cloth::GenerateBuffer()
{
	int i = 0;

	for(int x = 0; x<numParticlesWidth-1; x++)
	{
		for(int y=0; y<numParticlesHeight-1; y++)
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

			triangles.push_back(Triangle(shaderID, GetParticle(x+1,y), GetParticle(x,y),GetParticle(x,y+1), i));
			i++;
			triangles.push_back(Triangle(shaderID, GetParticle(x+1,y+1),GetParticle(x+1,y),GetParticle(x,y+1), i));
			i++;
		}
	}

	for(int i = 0; i < triangles.size(); i++)
	{
		triangles[i].SetColor(orderedColors[i]);
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
	int i = 0;

	//if(reset == true)
	//{
	//	Reset();
	//	reset = false;
	//}
	// reset normals (which where written to last frame)
	std::vector<Particle>::iterator particle;
	for(particle = particles.begin(); particle != particles.end(); particle++)
	{
		(*particle).ResetNormal();
	}

	//create smooth per particle normals by adding up all the (hard) triangle normals that each particle is part of
	for(int x = 0; x<numParticlesWidth-1; x++)
	{
		for(int y=0; y<numParticlesHeight-1; y++)
		{
			glm::vec3 normal = CalcNormal(GetParticle(x+1,y),GetParticle(x,y),GetParticle(x,y+1));
			GetParticle(x+1,y)->AddToNormal(normal);
			GetParticle(x,y)->AddToNormal(normal);
			GetParticle(x,y+1)->AddToNormal(normal);

			normal = CalcNormal(GetParticle(x+1,y+1),GetParticle(x+1,y),GetParticle(x,y+1));
			GetParticle(x+1,y+1)->AddToNormal(normal);
			GetParticle(x+1,y)->AddToNormal(normal);
			GetParticle(x,y+1)->AddToNormal(normal);
		}
	}


	if(movable == true)
	{
		DropCloth();
	}
	Mode temp = previousMode;
	Mode currentMode = mode;

	if(temp != currentMode)
	{
		ChangeMode(currentMode);
	}

	orderedParticles.clear();

	for(int x = 0; x<numParticlesWidth-1; x++)
	{
		for(int y=0; y<numParticlesHeight-1; y++)
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

void Cloth::ChangeMode(Mode mode)
{
	previousMode = mode;

	if(mode == CLOTH_HANGING)
	{
		for(int i=0;i<3; i++)
		{
			GetParticle(0+i ,0)->OffsetPosition(glm::vec3(0.5,0.0,0.0)); // moving the particle a bit towards the center, to make it hang more natural
			GetParticle(0+i ,0)->MakeUnmovable();

			GetParticle(0+i ,0)->OffsetPosition(glm::vec3(-0.5,0.0,0.0)); // moving the particle a bit towards the center, to make it hang more natural
			GetParticle(numParticlesWidth-i-1,0)->MakeUnmovable();
		}

		for(int i = 0; i < 3; i++)
		{
			//GetParticle(0 ,numParticlesHeight-i-1)->OffsetPosition(glm::vec3(0,height,height));// moving the particle a bit towards the center, to make it hang more natural
			GetParticle(0 ,numParticlesHeight-i-1)->SetFixed(false);

			//GetParticle(numParticlesWidth-i-1 ,numParticlesHeight-i-1)->OffsetPosition(glm::vec3(0,height,height));
			GetParticle(numParticlesWidth-i-1 ,numParticlesHeight-i-1)->SetFixed(false);
		}
	}
	else
	{
		// making the upper left most three and right most three particles unmovable
		for(int i=0;i<3; i++)
		{
			GetParticle(0+i ,0)->OffsetPosition(glm::vec3(0.5,0.0,0.0)); // moving the particle a bit towards the center, to make it hang more natural
			GetParticle(0+i ,0)->MakeUnmovable();

			GetParticle(0+i ,0)->OffsetPosition(glm::vec3(-0.5,0.0,0.0)); // moving the particle a bit towards the center, to make it hang more natural
			GetParticle(numParticlesWidth-i-1 ,0)->MakeUnmovable();
		}

		for(int i = 0; i < 1; i++)
		{
			GetParticle(0 ,numParticlesHeight-i-1)->OffsetPosition(glm::vec3(0,height,height));// moving the particle a bit towards the center, to make it hang more natural
			GetParticle(0 ,numParticlesHeight-i-1)->MakeUnmovable();

			GetParticle(numParticlesWidth-i-1 ,numParticlesHeight-i-1)->OffsetPosition(glm::vec3(0,height,height));
			GetParticle(numParticlesWidth-i-1 ,numParticlesHeight-i-1)->MakeUnmovable();
		}
	}
}

void Cloth::DropCloth()
{
	// making the upper left most three and right most three particles unmovable
	for(int i=0;i<3; i++)
	{
		GetParticle(0+i ,0)->OffsetPosition(glm::vec3(0.5,0.0,0.0)); // moving the particle a bit towards the center, to make it hang more natural
		GetParticle(0+i ,0)->SetFixed(false);

		GetParticle(0+i ,0)->OffsetPosition(glm::vec3(-0.5,0.0,0.0)); // moving the particle a bit towards the center, to make it hang more natural
		GetParticle(numParticlesWidth-i-1 ,0)->SetFixed(false);
	}

	for(int i = 0; i < 3; i++)
	{
		GetParticle(0 ,numParticlesHeight-i-1)->SetFixed(false);
		GetParticle(numParticlesWidth-i-1 ,numParticlesHeight-i-1)->SetFixed(false);
	}
}

void Cloth::Reset()
{
	particles.clear();
	constraints.clear();

	//int i = 0;
	//for(int x=0; x<numParticlesWidth; x++)
	//{
	//	for(int y=0; y<numParticlesHeight; y++)
	//	{

	//		glm::vec3 pos = glm::vec3(width * (x/(float)numParticlesWidth),-height * (y/(float)numParticlesHeight),0);
	//		particles[y*numParticlesWidth+x] = Particle(pos, i); // insert particle in column x at y'th row
	//		i++;
	//	}
	//}

	//// Connecting immediate neighbor particles with constraints (distance 1 and sqrt(2) in the grid)
	//for(int x=0; x<numParticlesWidth; x++)
	//{
	//	for(int y=0; y<numParticlesHeight; y++)
	//	{
	//		if (x<numParticlesWidth -1) SetConstraint(GetParticle(x,y),GetParticle(x+1,y));
	//		if (y<numParticlesHeight -1) SetConstraint(GetParticle(x,y),GetParticle(x,y+1));
	//		if (x<numParticlesWidth -1 && y<numParticlesHeight-1) SetConstraint(GetParticle(x,y),GetParticle(x+1,y+1));
	//		if (x<numParticlesWidth -1 && y<numParticlesHeight-1) SetConstraint(GetParticle(x+1,y),GetParticle(x,y+1));
	//	}
	//}

	//// Connecting secondary neighbors with constraints (distance 2 and sqrt(4) in the grid)
	//for(int x=0; x < numParticlesWidth; x++)
	//{
	//	for(int y=0; y < numParticlesHeight; y++)
	//	{
	//		if (x<numParticlesWidth-2) SetConstraint(GetParticle(x,y),GetParticle(x+2,y));
	//		if (y<numParticlesHeight-2) SetConstraint(GetParticle(x,y),GetParticle(x,y+2));
	//		if (x<numParticlesWidth-2 && y<numParticlesHeight-2) SetConstraint(GetParticle(x,y),GetParticle(x+2,y+2));
	//		if (x<numParticlesWidth-2 && y<numParticlesHeight-2) SetConstraint(GetParticle(x+2,y),GetParticle(x,y+2));			
	//	}
	//}

	//ChangeMode(CLOTH_HANGING);
}

bool Cloth::checkNarrowPhaseCollision(Triangle triangle, Triangle triangle2)
{
	glm::vec3 dir = triangle.p1->position - triangle2.p1->position;

	glm::vec3 farA = GetFarthestPointInDirection(dir, triangle.points);
	glm::vec3 farB = GetFarthestPointInDirection(-dir, triangle2.points);

	glm::vec3 supportPoint = farA - farB;

	std::vector<glm::vec3> simplex;
	simplex.push_back(supportPoint);

	dir = -supportPoint;

	int count = 0;

	while(count < 100)
	{
		farA = GetFarthestPointInDirection(dir, triangle.points);
		farB = GetFarthestPointInDirection(-dir, triangle2.points);

		supportPoint = farA - farB;

		if(glm::dot(supportPoint,dir) < 0)
		{
			return false;
		}

		simplex.push_back(supportPoint);

		if(simplexContainsOrigin(simplex,dir))
		{
			return true;
		}

		count++;
	}

	return false;
}

bool Cloth::simplexContainsOrigin(std::vector<glm::vec3> &simplex, glm::vec3 &dir)
{
			glm::vec3 a,b,c,d;
		
			// Based on video: http://mollyrocket.com/849
			if(simplex.size() == 2)
			{
				a = simplex[1];
				b = simplex[0];
		
				glm::vec3 ab = b - a;
				glm::vec3 ao = -a;
		
				glm::vec3 abPerp = glm::cross(glm::cross(ab,ao),ab);
		
				if(glm::dot(ab,ao) > 0)
				{
					dir = abPerp;
				}
				else
				{
					dir = ao;
				}
		
				return false;
			}
			else if(simplex.size() == 3)
			{
				return checkTriangle(simplex,dir);
			}
			// Helpful site of tetrahedron: http://in2gpu.com/2014/05/18/gjk-algorithm-3d/
			else if(simplex.size() == 4)
			{
				a = simplex[3];
				b = simplex[2];
				c = simplex[1];
				d = simplex[0];
		
				glm::vec3 ao = -a;
		
				glm::vec3 ab = b - a;
				glm::vec3 ac = c - a;
				glm::vec3 ad = d - a;
		
				glm::vec3 abc = glm::cross(ab, ac);
				glm::vec3 acd = glm::cross(ac, ad);
				glm::vec3 adb = glm::cross(ad, ab);
		
				glm::vec3 abPerp = glm::cross(glm::cross(ab,ao),ab);
				glm::vec3 acPerp = glm::cross(glm::cross(ac,ao),ac);
				glm::vec3 ab_abc = glm::cross(ab,abc);
				glm::vec3 abc_ac = glm::cross(abc,ac);
		
				// In front of abc face
				if(glm::dot(abc,ao) > 0)
				{	
					simplex.erase(simplex.begin()); // erase d
		
					return checkTriangle(simplex,dir);
				}
				// In front of ACD face
				else if(glm::dot(acd,ao) > 0)
				{
					simplex.erase(simplex.begin() + 2); // erase b
			
					return checkTriangle(simplex,dir);
				}
				// in front of ADB face
				else if(glm::dot(adb,ao) > 0)
				{
					simplex.erase(simplex.begin() + 1); // erase c
					simplex[0] = b;
					simplex[1] = d;
		
					return checkTriangle(simplex,dir);
				}
	
				// NB. origin cannot be below cbd because we already checked that
		
				return true;
		
			}
}

glm::vec3 Cloth::GetFarthestPointInDirection(glm::vec3 dir, std::vector<glm::vec3> points)
{
	glm::vec3 farthestPoint = points[0];
	float farthest = glm::dot(points[0],dir);
	float temp;

	for(int i = 1; i <points.size();i++)
	{
		temp = glm::dot(dir,points[i]);

		if(temp > farthest)
		{
			farthest = temp;
			farthestPoint = points[i];
		}
	}

	return farthestPoint;
}

bool Cloth::checkTriangle(std::vector<glm::vec3> &simplex,glm::vec3 &dir)
{
	glm::vec3 c = simplex[0];
	glm::vec3 b = simplex[1];
	glm::vec3 a = simplex[2];
	
	glm::vec3 ao = -a;
	
	glm::vec3 ab = b - a;
	glm::vec3 ac = c - a;
	
	glm::vec3 abc = glm::cross(ab, ac);
	
	glm::vec3 abPerp = glm::cross(glm::cross(ab,ao),ab);
	glm::vec3 acPerp = glm::cross(glm::cross(ac,ao),ac);
	glm::vec3 ab_abc = glm::cross(ab,abc);
	glm::vec3 abc_ac = glm::cross(abc,ac);
	
	if(glm::dot(abc_ac,ao) > 0)
	{
		if(glm::dot(ac,ao) > 0)
		{
			dir = acPerp;
			simplex.erase(simplex.begin() + 1); // erase b
		}
		else
		{
			if(glm::dot(ab,ao) > 0)
			{
				dir = abPerp;
				simplex.erase(simplex.begin()); // erase c
			}
			else
			{
				dir = ao;
				simplex.erase(simplex.begin()); // erase c
				simplex.erase(simplex.begin()); // erase b
			}
		}
	}
	else
	{
		if(glm::dot(ab_abc,ao) > 0) // AB plane 
		{
			if(glm::dot(ab,ao) > 0)
			{
				dir = abPerp;
				simplex.erase(simplex.begin()); // erase c
			}
			else
			{
				dir = ao;
				simplex.erase(simplex.begin()); // erase c
				simplex.erase(simplex.begin()); // erase b
			}
		}
		else
		{
			if(glm::dot(abc, ao) > 0) // outside face
			{
				dir = abc;
			}
			else // inside face
			{
				simplex[0] = b;
				simplex[1] = c;
	
				dir = -abc;
			}
		}
	}
	
	return false;
}
