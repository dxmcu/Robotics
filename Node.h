/*
 * @file	Node.h
 *
 * @author	Yuval Goldberg
 * @since	06/07/2017
 */

#ifndef NODE_H_
#define NODE_H_

class Node
{
public:
	Node(int xp, int yp, int d, int p);
	Node(int xp, int yp);

	int GetX() const;
	int GetY() const;
	int GetLevel() const;
	int GetPriority() const;
	void UpdatePriority(const int & xDest, const int & yDest);

	void nextLevel(const int & i, int cellValue);

	const int & estimate(const int & xDest, const int & yDest) const;

private:

	/**
	 * Current position
	 */
	int m_X;
	int m_Y;

	// total distance already traveled to reach the node
	int m_level;

	// priority=level+remaining distance estimate
	int m_priority;  // smaller: higher m_priority
};

#endif /* NODE_H_ */
