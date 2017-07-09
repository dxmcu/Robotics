/*
 * @file	PathPlanner.h
 *
 * @author	Yuval Goldberg
 * @since	07/06/2017
 */

#ifndef PATHPLANNER_H_
#define PATHPLANNER_H_

#include <HamsterAPIClientCPP/Hamster.h>
#include "Map.h"
#include "Typedefs.h"

using namespace std;
using namespace HamsterAPI;

#define NUMBER_OF_POSSIBLES_DIRECTIONS	8

class PathPlanner
{
public:
	PathPlanner(Map* map, int startRow, int startCol, int targetRow, int targetCol);

	Path* ComputeShortestPath();

private:
	void ResetPlannerMaps();
	string AStarShortestPath();

private:
	OccupancyGrid* m_grid;
	int m_startRow, m_startCol, m_targetRow, m_targetCol;

	// Maps
	Map* m_map;
	ECellType** m_logicMap;
	int** m_closedNodesMap;
	int** m_openNodesMap;
	int** m_dirMap;

	static const int X_DIRECTIONS[NUMBER_OF_POSSIBLES_DIRECTIONS];
	static const int Y_DIRECTIONS[NUMBER_OF_POSSIBLES_DIRECTIONS];
};

#endif /* PATHPLANNER_H_ */
