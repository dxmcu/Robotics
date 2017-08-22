/*
 * @file	Particle.cpp
 *
 * @author	Yuval Goldberg
 * @since	09/08/2017
 */

#include <stdlib.h>
#include <math.h>
#include "Particle.h"

Particle::Particle(Position pos, double belief, Map* map, bool toRandom) :
					belief(belief), m_map(map)
{
	this->pos.x = pos.x;
	this->pos.y = pos.y;
	this->pos.yaw = pos.yaw;

	if (toRandom)
	{
		Randomize();
	}
}

Particle::~Particle()
{
	m_map->GetMap()[(int)pos.y][(int)pos.x] = eCellType_emptyCell;
}

Particle Particle::CreateAnotherParticle() const
{
	return Particle(pos, belief * 0.9, m_map);
}

void Particle::Randomize()
{
	Position tempPos(pos.x, pos.y, pos.yaw);

	int counterDebug = 0;

	/* Run until getting a free cell */
	do
	{
		tempPos = CreateRandomPos(pos);

		if (counterDebug % 50 == 0)
		{
			printf("Temp pos: (%f,%f)\n", tempPos.x, tempPos.y);
		}

		counterDebug++;

	} while (m_map->GetMap()[(int)tempPos.y][(int)tempPos.x] != eCellType_emptyCell);

	pos.x = tempPos.x;
	pos.y = tempPos.y;
	pos.yaw = tempPos.yaw;

	m_map->GetMap()[(int)pos.y][(int)pos.x] = eCellType_particleCell;
}

Position Particle::CreateRandomPos(Position currPos)
{
	if (currPos.x < 0 || currPos.x >= m_map->GetWidth())
	{
		currPos.x = 0;
	}

	if (currPos.y < 0 || currPos.y >= m_map->GetHeight())
	{
		currPos.y = 0;
	}

	// Calc range of delta particle
	int rowRange = floor(m_map->GetHeight() / 2);
	int colRange = floor(m_map->GetWidth() / 2);
	double YawRange = M_PI * 2 / 2 * 0.1;
	int intYawRange = YawRange * 10000;
	double deltaRow = 0, deltaCol = 0, deltaYaw = 0;

	do
	{
		deltaRow = (rand() % (2 * rowRange)) - rowRange;
		deltaCol = (rand() % (2 * colRange)) - colRange;
		deltaYaw = (rand() % (2 * intYawRange)) - intYawRange;
		deltaYaw = deltaYaw / 10000;
	} while (!m_map->IsValidPoint(currPos.y + deltaRow, currPos.x + deltaCol));

	return Position(currPos.x + deltaCol, currPos.y + deltaRow, currPos.yaw + deltaYaw);
}

double Particle::Update(Position deltaPos)
{
	printf("Update current particle\n");

	if (!m_map->IsValidPoint(pos.y + deltaPos.y, pos.x + deltaPos.x))
	{
		return 0;
	}

	/* Chaning the position of the particle */
	m_map->GetMap()[(int)pos.y][(int)pos.x] = eCellType_emptyCell;

	printf("Curr pos %f %f\n", pos.x, pos.y);
	printf("Setting deltas %f %f\n", deltaPos.x, deltaPos.y);

	pos.x += deltaPos.x;
	pos.y += deltaPos.y;
	pos.yaw += deltaPos.yaw;

	printf("Getting map second time %d %d\n", (int)pos.x, (int)pos.y);

	m_map->GetMap()[(int)pos.y][(int)pos.x] = eCellType_particleCell;

	belief = 1.1 * ProbByMove(deltaPos) * belief;

	/* Max */
	if (belief > 1)
	{
		belief = 1;
	}

	printf("FINISH Update current particle\n");

	return belief;
}

double Particle::ProbByMove(Position deltaPos)
{
	printf("Probe by move\n");

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

	printf("FINISH Probe by move\n");

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
