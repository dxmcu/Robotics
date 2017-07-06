/*
 * @file	Node.cpp
 *
 * @author	Yuval Goldberg
 * @since	06/07/2017
 */

#include <cmath>
#include "Node.h"

Node::Node(int xp, int yp, int d, int p) : m_X(xp), m_Y(yp), m_level(d), m_priority(p) {}

Node::Node(int xp, int yp) : m_X(xp), m_Y(yp), m_level(0), m_priority(0) {}

int Node::getxPos() const
{
	return m_X;
}

int Node::getyPos() const
{
	return m_Y;
}

int Node::getLevel() const
{
	return m_level;
}

int Node::getPriority() const
{
	return m_priority;
}

void Node::updatePriority(const int & xDest, const int & yDest)
{
	// A*
	m_priority = m_level + estimate(xDest, yDest);
}

// TODO: Give proprity based on distance from blocked

// Better priority for going forward instead of diagonally
void Node::nextLevel(const int & i, int cellValue) // i: direction
{
	//level += (NUMBER_OF_POSSIBLES_DIRECTIONS == 8 ? (i % 2 == 0 ? 10 : 14) : 10);
	m_level += (i % 2 == 0) ? 10 : 14;
	m_level += cellValue;
}

// Estimation function for the remaining distance to the goal.
const int &Node::estimate(const int & xDest, const int & yDest) const
{
	static int xd, yd, d;
	xd = xDest - m_X;
	yd = yDest - m_Y;

	// Euclidian Distance
	d = static_cast<int>(sqrt(xd * xd + yd * yd));

	return d;
}


