/*
 * @file	WaypointsManager.h
 *
 * @author	Yuval Goldberg
 * @since	09/07/2017
 */

#ifndef WAYPOINTSMANAGER_H_
#define WAYPOINTSMANAGER_H_

#include <vector>
#include "Map.h"
#include "Typedefs.h"

using namespace std;

class WaypointsManager
{
public:
	vector<Position> waypoints;

	WaypointsManager(Map* map, Path* path);

	void GenerateWaypoints();
	void PrintWaypoints() const;

private:
	/**
	 * Implementing the algorithm from Milestone 4 to move between waypoints
	 * without collisions with obstacles.
	 */
	void Raytrace(int x0, int y0, int x1, int y1);

	double GetAngle(double x1, double y1, double x2, double y2) const;

private:
	Map* m_map;
	Path* m_path;

	static const uint32_t DISTANCE_BETWEEN_WAYPOINTS = 3;
};

#endif /* WAYPOINTSMANAGER_H_ */
