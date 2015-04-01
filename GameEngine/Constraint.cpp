#include "Constraint.h"

/************************************************************************/
/* Tutorial used: http://cg.alexandra.dk/?p=147                          */
/************************************************************************/

Constraint::Constraint(Particle *p1, Particle *p2)
{
	this->p1 = p1;
	this->p2 = p2;

	initialDistance = glm::length(this->p1->GetPos() - this->p2->GetPos());
	currentDistance = initialDistance;
}

void Constraint::HandleContraint()
{
	glm::vec3 temp = p2->GetPos() - p1->GetPos();

	currentDistance = glm::length(temp);
	glm::vec3 correctionVector = temp * (1 - initialDistance / currentDistance); // The offset vector that could moves p1 into a distance of rest_distance to p2
	glm::vec3 correctionVectorHalf = correctionVector * 0.5f; // Lets make it half that length, so that we can move BOTH p1 and p2.
	p1->OffsetPosition(correctionVectorHalf); // correctionVectorHalf is pointing from p1 to p2, so the length should move p1 half the length needed to satisfy the constraint.
	p2->OffsetPosition(-correctionVectorHalf); // we must move p2 the negative direction of correctionVectorHalf since it points from p2 to p1, and not p1 to p2.	
}

bool Constraint::CompareParticles(Particle *p)
{
	if(p == p1 || p == p2)
	{
		return true;
	}
	else
	{
		return false;
	}
}
