/*
 * @file	LocalizationManager.cpp
 *
 * @author	Yuval Goldberg
 * @since	09/08/2017
 */

#include <iostream>
#include <algorithm>
#include "LocalizationManager.h"

using namespace std;

LocalizationManager::LocalizationManager(Position start, Map* map, int amount)
{
	m_map = map;
	m_amountOfParticles = amount;

	srand(time(NULL));

	m_currPos = Position(start.x, start.y, start.yaw);

	Particle startParticle(m_currPos, (double)1, m_map, false);
	CreateParticles(startParticle);
}

void LocalizationManager::CreateParticles(const Particle fromParticle)
{
	printf("Create particles \n");

	/* Add start */
	if (m_lstOfParticles.size() == 0)
	{
		printf("Empty list\n");
		m_lstOfParticles.push_back(fromParticle);
	}

	printf("Amonut needed: %u\n", m_amountOfParticles - (unsigned) m_lstOfParticles.size());

	/* Create all the particles from the fromParticle */
	for (uint32_t index = 0; index < m_amountOfParticles - (unsigned) m_lstOfParticles.size(); index++)
	{
		m_lstOfParticles.push_back(fromParticle.CreateAnotherParticle());
	}
	printf("FINISH create\n");

}

Particle LocalizationManager::GetBestParticle()
{
	Particle bestParticle = m_lstOfParticles[0];

	printf("Getting the best!!!\n");

	/* Go over all the particles */
	for (uint32_t index = 0; index < m_lstOfParticles.size(); index++)
	{
		// check if we got better particle
		if (m_lstOfParticles[index] > bestParticle)
		{
			bestParticle = m_lstOfParticles[index];
		}
	}

	printf("FINISH Getting the best!!!\n");

	return bestParticle;
}

void LocalizationManager::UpdateAllParticles(Position* deltaPosition)
{
	// store the position for restore
	m_currPos.x += deltaPosition->x;
	m_currPos.y += deltaPosition->y;
	m_currPos.yaw += deltaPosition->yaw;

	printf("Going over size %u\n", m_lstOfParticles.size());
	int debugCounter = 0;

	/* Go over all the particles and update their belief */
	for (vector<Particle>::iterator currParticle = m_lstOfParticles.begin(); currParticle != m_lstOfParticles.end();)
	{
		printf("Curr particle %u\n", debugCounter);

		if (currParticle->Update(*deltaPosition) < 0.4)
		{
			printf("Erase\n");
			m_lstOfParticles.erase(currParticle);
		}
		else
		{
			printf("Increase\n");
			currParticle++;
		}

		debugCounter++;
	}

	printf("Creating curr size %u\n", m_lstOfParticles.size());

	/* Fill the needed particles */
	if (m_lstOfParticles.size() == 0)
	{
		printf("Create new\n");
		Particle startParticle(m_currPos, (double)1, m_map, false);
		printf("With start particle\n");
		CreateParticles(startParticle);
	}
	else
	{
		CreateParticles(GetBestParticle());
	}

	printf("FINISH update\n");

}

vector<Particle> LocalizationManager::GetParticles() const
{
	return m_lstOfParticles;
}

void LocalizationManager::PrintParticles() const
{
	for (uint32_t index = 0; index < m_lstOfParticles.size(); index++)
	{
		Particle currPar = m_lstOfParticles[index];
		printf("Particle(%f, %f, %f, %f)\n", currPar.pos.x, currPar.pos.y, currPar.pos.yaw, currPar.belief);
	}
}
