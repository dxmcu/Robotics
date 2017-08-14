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
};

#endif /* PATHPLANNER_H_ */
