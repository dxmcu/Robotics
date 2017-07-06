/*
 * @file	PathPlanner.cpp
 *
 * @author	Yuval Goldberg
 * @since	07/06/2017
 */

#include <queue>
#include "Node.h"
#include "PathPlanner.h"

const int PathPlanner::DX_DIRECTION[NUMBER_OF_POSSIBLES_DIRECTIONS] = { 1, 1, 0, -1, -1, -1, 0, 1 };
const int PathPlanner::DY_DIRECTION[NUMBER_OF_POSSIBLES_DIRECTIONS] = { 0, 1, 1, 1, 0, -1, -1, -1 };

// Determine priority (in the priority queue)
bool operator<(const Node & a, const Node & b)
{
	return a.GetPriority() > b.GetPriority();
}

PathPlanner::PathPlanner(Map* map, int startRow, int startCol, int targetRow, int targetCol) :
							m_startRow(startRow), m_startCol(startCol), m_targetRow(targetRow), m_targetCol(targetCol), m_map(map)
{
	m_grid = m_map->GetCoarseGrid();
	m_logicMap = m_map->GetMap();

	m_openNodesMap = new int*[m_grid->getHeight()];
	m_closedNodesMap = new int*[m_grid->getHeight()];
	m_dirMap = new int*[m_grid->getHeight()];

	/* Init maps */
	for (uint32_t row = 0; row < m_grid->getHeight(); row++)
	{
		m_openNodesMap[row] = new int[m_grid->getWidth()];
		m_closedNodesMap[row] = new int[m_grid->getWidth()];
		m_dirMap[row] = new int[m_grid->getWidth()];
	}
}

string PathPlanner::AStarShortestPath()
{
	// list of open (not-yet-tried) nodes
	priority_queue<Node> pq[2];
	Node* n0 = NULL;
	Node* m0 = NULL;
	int i = 0, j = 0, x = 0, y = 0, xdx = 0, ydy = 0, pqi = 0;
	char c;

	ResetPlannerMaps();

	// create the start node and push into list of open nodes
	n0 = new Node(m_startRow, m_startCol, 0, 0);

	// get the priority of the first node
	n0->UpdatePriority(m_targetRow, m_targetCol);
	pq[pqi].push(*n0);
	// mark it on the open nodes MyMap
	m_openNodesMap[x][y] = n0->GetPriority();

	// A* search
	while (!pq[pqi].empty())
	{
		// get the current node w/ the highest priority
		// from the list of open nodes
		n0 = new Node(pq[pqi].top().GetX(), pq[pqi].top().GetY(),
				pq[pqi].top().GetLevel(), pq[pqi].top().GetPriority());

		x = n0->GetX();
		y = n0->GetY();

		// remove the node from the open list
		pq[pqi].pop();

		m_openNodesMap[x][y] = 0;

		// mark it on the closed nodes MyMap
		m_closedNodesMap[x][y] = 1;

		// quit searching when the goal state is reached
		//if((*n0).estimate(m_goalRow, m_goalCol) == 0)
		if (x == m_targetRow && y == m_targetCol)
		{
			// generate the path from finish to start
			// by following the directions
			string path = "";
			while (!(x == m_startRow && y == m_startCol))
			{
				j = m_dirMap[x][y];
				c = '0' + (j + NUMBER_OF_POSSIBLES_DIRECTIONS / 2) % NUMBER_OF_POSSIBLES_DIRECTIONS;
				path = c + path;
				x += DX_DIRECTION[j];
				y += DY_DIRECTION[j];
			}

			// garbage collection
			delete n0;

			// empty the leftover nodes
			while (!pq[pqi].empty())
			{
				pq[pqi].pop();
			}

			return path;
		}

		// generate moves (child nodes) in all possible directions
		for (i = 0; i < NUMBER_OF_POSSIBLES_DIRECTIONS; i++)
		{
			xdx = x + DX_DIRECTION[i];
			ydy = y + DY_DIRECTION[i];

			// check if out of bounds and
			if (!(xdx < 0 || xdx > m_grid->getHeight() - 1 || ydy < 0 || ydy > m_grid->getWidth() - 1
					|| m_logicMap[xdx][ydy] == 1 || m_closedNodesMap[xdx][ydy] == 1))
			{
				// generate a child node
				m0 = new Node(xdx, ydy, n0->GetLevel(), n0->GetPriority());
				m0->nextLevel(i, m_logicMap[xdx][ydy]);
				m0->UpdatePriority(m_targetRow, m_targetCol);

				// if it is not in the open list then add into that
				if (m_openNodesMap[xdx][ydy] == 0)
				{
					m_openNodesMap[xdx][ydy] = m0->GetPriority();
					pq[pqi].push(*m0);

					// mark its parent node direction
					m_dirMap[xdx][ydy] = (i + NUMBER_OF_POSSIBLES_DIRECTIONS / 2) % NUMBER_OF_POSSIBLES_DIRECTIONS;
				}
				else if (m_openNodesMap[xdx][ydy] > m0->GetPriority())
				{
					// update the priority info
					m_openNodesMap[xdx][ydy] = m0->GetPriority();

					// update the parent direction info
					m_dirMap[xdx][ydy] = (i + NUMBER_OF_POSSIBLES_DIRECTIONS / 2) % NUMBER_OF_POSSIBLES_DIRECTIONS;

					// replace the node
					// by emptying one pq to the other one
					// except the node to be replaced will be ignored
					// and the new node will be pushed in instead
					while (!(pq[pqi].top().GetX() == xdx
							&& pq[pqi].top().GetY() == ydy))
					{
						pq[1 - pqi].push(pq[pqi].top());
						pq[pqi].pop();
					}

					// remove the wanted nodeusing namespace std;
					pq[pqi].pop();

					// empty the larger size pq to the smaller one
					if (pq[pqi].size() > pq[1 - pqi].size())
						pqi = 1 - pqi;
					while (!pq[pqi].empty())
					{
						pq[1 - pqi].push(pq[pqi].top());
						pq[pqi].pop();
					}
					pqi = 1 - pqi;
					pq[pqi].push(*m0); // add the better node instead
				} else
					delete m0; // garbage collection
			}

		}

		delete n0; // garbage collection
	}
	return ""; // no route found
}

Path* PathPlanner::ComputeShortestPath()
{
	Path* route = NULL;
	Node** arrPathNodes;
	string path = AStarShortestPath();

	if (path != "")
	{
		cout << "Computed shortest path: " << path << endl;
	}
	else
	{
		cout << "An empty route generated!" << endl;
		return NULL;
	}

	if (path.length() > 0)
	{
		arrPathNodes = new Node*[path.length()];

		for (uint32_t i = 0; i < path.length(); i++)
		{
			arrPathNodes[i] = NULL;
		}

		int pathX = m_startRow;
		int pathY = m_startCol;

		m_logicMap[m_startRow][m_startCol] = eCellType_startCell;

		// build the path to MyMap and in arrPathNodes
		for (uint32_t i = 0; i < path.length(); i++)
		{
			char c = path.at(i);
			int j = atoi(&c);
			pathX += DX_DIRECTION[j];
			pathY += DY_DIRECTION[j];
			m_logicMap[pathX][pathY] = eCellType_pathCell;
			arrPathNodes[i] = new Node(pathX, pathY);
		}

		m_logicMap[pathX][pathY] = eCellType_endCell;

		route = new Path;
		route->computedPath = arrPathNodes;
		route->sizeOfPath = path.length();
	}

	return route;
}

void PathPlanner::ResetPlannerMaps()
{
	for (uint32_t row = 0; row < m_grid->getHeight(); row++)
	{
		for (uint32_t col = 0; col < m_grid->getWidth(); col++)
		{
			m_openNodesMap[row][col] = 0;
			m_closedNodesMap[row][col] = 0;
		}
	}
}
