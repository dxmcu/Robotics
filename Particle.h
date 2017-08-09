/*
 * @file	Node.h
 *
 * @author	Yuval Goldberg
 * @since	09/08/2017
 */

#ifndef PARTICLE_H_
#define PARTICLE_H_

#include "Map.h"

class Particle
{
public:
	Position  pos;
	double belief;

	/* CTORs */
	Particle(Position pos, double belief, Map* map);

	Particle CreateAnotherParticle() const;

	double Update(Position deltaPos);
	double ProbByMove(Position deltaPos);

	/* Operators */
	bool operator>(const Particle &other) const;
	bool operator==(const Particle &other) const;

private:
	void Randomize();
	void CreateRandomPos();

private:
	Map* m_map;

};

#endif /* PARTICLE_H_ */
