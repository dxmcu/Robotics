/*
 * @file	WaypointsManager.cpp
 *
 * @author	Yuval Goldberg
 * @since	09/07/2017
 */

#include <iostream>
#include <cmath>
#include "WaypointsManager.h"

using namespace std;

WaypointsManager::WaypointsManager(Map* map, Path* path) : m_map(map), m_path(path)
{
}

void WaypointsManager::GenerateWaypoints()
{
	/* Creating waypoints over the path */
	for (uint32_t pathIndex = 0; pathIndex < m_path->length - DISTANCE_BETWEEN_WAYPOINTS; pathIndex += DISTANCE_BETWEEN_WAYPOINTS)
	{
		Raytrace(m_path->computedPath[pathIndex]->GetX(),
				 m_path->computedPath[pathIndex]->GetY(),
				 m_path->computedPath[pathIndex + DISTANCE_BETWEEN_WAYPOINTS]->GetX(),
				 m_path->computedPath[pathIndex + DISTANCE_BETWEEN_WAYPOINTS]->GetY());
	}
}

void WaypointsManager::Raytrace(int x0, int y0, int x1, int y1)
{
	int dx = abs(x1 - x0);
	int dy = abs(y1 - y0);
	int x = x0;
	int y = y0;
	int n = 1 + dx + dy;
	int x_inc = (x1 > x0) ? 1 : -1;
	int y_inc = (y1 > y0) ? 1 : -1;
	int error = dx - dy;
	dx *= 2;
	dy *= 2;

	for (; n > 0; --n)
	{
		if (error > 0)
		{
			x += x_inc;
			error -= dy;
		}
		else
		{
			y += y_inc;
			error += dx;
		}
	}

	cout << "(" << x << "," << y << ")" << endl;
	(m_map->GetMap())[x][y] = eCellType_wayPointCell;
}
