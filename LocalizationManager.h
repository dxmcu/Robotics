/*
 * @file	LocalizationManager.h
 *
 * @author	Yuval Goldberg
 * @since	09/08/2017
 */

#ifndef LOCALIZATIONMANAGER_H_
#define LOCALIZATIONMANAGER_H_

#include <vector>
#include "Typedefs.h"
#include "Particle.h"

using namespace std;

class LocalizationManager
{
public:
	LocalizationManager(Position start, Map* map, int amount);

	Particle GetBestParticle();
	void CreateParticles(const Particle fromParticle);
	void UpdateAllParticles(Position* deltaPosition);

	vector<Particle> GetParticles() const;
	void PrintParticles() const;


private:
	vector<Particle> m_lstOfParticles;
	Position m_currPos;
	Map* m_map;
	int m_amountOfParticles;
};

#endif /* LOCALIZATIONMANAGER_H_ */
