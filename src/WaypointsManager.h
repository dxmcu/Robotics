/*
 * @file	WaypointsManager.h
 *
 * @author	Yuval Goldberg
 * @since	09/07/2017
 */

#ifndef WAYPOINTSMANAGER_H_
#define WAYPOINTSMANAGER_H_

#include "Map.h"
#include "Typedefs.h"

class WaypointsManager
{
public:
	WaypointsManager(Map* map, Path* path);

	void GenerateWaypoints();

private:
	/**
	 * Implementing the algorithm from Milestone 4 to move between waypoints
	 * without collisions with obstacles.
	 */
	void Raytrace(int x0, int y0, int x1, int y1);

private:
	Map* m_map;
	Path* m_path;

	static const uint32_t DISTANCE_BETWEEN_WAYPOINTS = 3;
};

#endif /* WAYPOINTSMANAGER_H_ */
