/*
 * @file	Main.cpp
 *
 * @author	Yuval Goldberg
 * @since	29/06/2017
 */

#include <stdio.h>
#include "ConfigurationManager/Configuration.h"
#include "WaypointsManager.h"
#include "PathPlanner.h"
#include "Robot.h"
#include "Map.h"

using namespace std;

#define DEFAULT_CONFIGURATION_FILE			"./Config.json"

int main(int argc, char ** argv)
{
	Path* path = NULL;

	try
	{
		Configuration* config = new Configuration();
		const char* configFile = argc < 2 ? DEFAULT_CONFIGURATION_FILE : argv[1];

		printf("Trying to read config file: %s\n", configFile);
		if (!config->readConfiguration(configFile))
		{
			printf("Failed to read configuration file\n");
			return -1;
		}

		Robot myRobot = Robot::GetInstance();
		myRobot.SetSize(config->size);

		OccupancyGrid grid = myRobot.GetOccupancyGrid();
		Map* map = new Map(grid, myRobot.GetSize());

		printf("Computing the shortest path\n");
		PathPlanner* pathPlanner = new PathPlanner(map, config->startX, config->startY, config->destX, config->destY);
		path = pathPlanner->ComputeShortestPath();

		printf("Generate Waypoints\n");
		WaypointsManager* waypointsManager = new WaypointsManager(map, path);
		waypointsManager->GenerateWaypoints();

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
