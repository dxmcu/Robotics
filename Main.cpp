/*
 * @file	Main.cpp
 *
 * @author	Yuval Goldberg
 * @since	29/06/2017
 */

#include <stdio.h>
#include "PathPlanner.h"
#include "Robot.h"
#include "Map.h"

using namespace std;

int main(int argc, char ** argv)
{
	Path* path;

	try
	{
		Robot myRobot = Robot::GetInstance();

		OccupancyGrid grid = myRobot.GetOccupancyGrid();
		Map* map = new Map(grid, myRobot.GetSize());

		PathPlanner* pl = new PathPlanner(map, 228, 278, 228, 308);

		printf("Computing the shortest path\n");
		path = pl->ComputeShortestPath();

		while (myRobot.IsConnected())
		{
			map->Show();
			//myRobot.MoveAround();
			sleep(0.5);
		}
	}
	catch (const HamsterAPI::HamsterError & connection_error)
	{
		HamsterAPI::Log::i("Main", connection_error.what());
	}

	return 0;
}
