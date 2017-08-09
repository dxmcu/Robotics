/*
 * @file	Particle.cpp
 *
 * @author	Yuval Goldberg
 * @since	09/08/2017
 */

#include <stdlib.h>
#include <math.h>
#include "Particle.h"

Particle::Particle(Position pos, double belief, Map* map) :
					belief(belief), m_map(map)
{
	this->pos.x = pos.x;
	this->pos.y = pos.y;
	this->pos.yaw = pos.yaw;
	Randomize();
}

Particle Particle::CreateAnotherParticle() const
{
	return Particle(pos, belief * 0.9, m_map);
}

void Particle::Randomize()
{
	/* Run until getting a free cell */
	do
	{
		CreateRandomPos();
	} while (m_map->GetMap()[(int)pos.x][(int)pos.y] != eCellType_emptyCell);
}

void Particle::CreateRandomPos()
{
	if (pos.x < 0 || pos.x >= m_map->GetWidth())
	{
		pos.x = 0;
	}

	if (pos.y < 0 || pos.y >= m_map->GetHeight())
	{
		pos.y = 0;
	}

	// Calc range of delta particle
	int rowRange = floor(m_map->GetHeight() / 2 * 0.3);
	int colRange = floor(m_map->GetWidth() / 2 * 0.3);
	double YawRange = M_PI * 2 / 2 * 0.1;
	int intYawRange = YawRange * 10000;

	int deltaRow = (rand() % (2 * rowRange)) - rowRange;
	int deltaCol = (rand() % (2 * colRange)) - colRange;
	double deltaYaw = (rand() % (2 * intYawRange)) - intYawRange;
	deltaYaw = deltaYaw / 10000;

	pos.x += deltaCol;
	pos.y += deltaRow;
	pos.yaw += deltaYaw;
}

double Particle::Update(Position deltaPos)
{
	pos.x += deltaPos.x;
	pos.y += deltaPos.y;
	pos.yaw += deltaPos.yaw;

	belief = 1.1 * ProbByMove(deltaPos) * belief;

	/* Max */
	if (belief > 1)
	{
		belief = 1;
	}

	return belief;
}

double Particle::ProbByMove(Position deltaPos)
{
	float distance = sqrt(pow(deltaPos.x, 2) + pow(deltaPos.y, 2));

	// Check if the robot moved really really little bit
	if (distance < 0.2 && abs(deltaPos.yaw) < 0.1)
	{
		return 0.9;
	}
	if (distance < 0.4 && abs(deltaPos.yaw) < 0.2)
	{
		return 0.7;
	}
	if (distance < 0.7 && abs(deltaPos.yaw) < 0.3)
	{
		return 0.4;
	}

	return 0.3;
}

/**
 * Operators
 */

bool Particle::operator>(const Particle &other) const
{
	return (belief > other.belief);
}

bool Particle::operator==(const Particle &other) const
{
	return belief == other.belief &&
		   pos.x == other.pos.x   &&
		   pos.y == other.pos.y   &&
		   pos.yaw == other.pos.yaw;
}
