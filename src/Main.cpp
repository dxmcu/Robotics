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
#include "LocalizationManager.h"

using namespace std;

#define DEFAULT_CONFIGURATION_FILE			"./Config.json"
#define AMOUNT_OF_PARTICLES					(40)

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
		myRobot.SetInitPose(config->startX, config->startY, config->startYaw);

		OccupancyGrid grid = myRobot.GetOccupancyGrid();
		Map* map = new Map(grid, myRobot.GetSize());

		printf("Computing the shortest path\n");
		PathPlanner* pathPlanner = new PathPlanner(map, config->startX, config->startY, config->destX, config->destY);
		path = pathPlanner->ComputeShortestPath();

		printf("Generate Waypoints\n");
		WaypointsManager* waypointsManager = new WaypointsManager(map, path);
		waypointsManager->GenerateWaypoints();
		waypointsManager->PrintWaypoints();

		printf("Generate particles\n");
		Position startPosition(config->startX, config->startY, config->startYaw);
		LocalizationManager* localizationManager = new LocalizationManager(startPosition, map, AMOUNT_OF_PARTICLES);
		localizationManager->PrintParticles();

#if 0

		/* Going through all the waypoints */
		for (uint32_t index = 0; index < waypointsManager->waypoints.size(); index++)
		{
			printf("Waypoint No. %u\n", index);

			Pose currPos;
			myRobot.SetAngle(waypointsManager->waypoints[index].yaw);

			do
			{
				printf("Moving\n");
				myRobot.Move();
				printf("Updating\n");
				myRobot.Update(localizationManager);
				currPos = myRobot.GetPosition();

				printf("FINISH while loop\n");
			} while ((map->GetMap())[(int)currPos.getY()][(int)currPos.getX()] != eCellType_wayPointCell);
		}

		printf("FINISH\n");

#endif

		printf("Show map\n");
		map->Show();


		while (myRobot.IsConnected())
		{
			try
			{
				map->Show();
				myRobot.MoveAround();
				sleep(0.5);
			} catch (const HamsterAPI::HamsterError & message_error) {
				HamsterAPI::Log::i("Connected", message_error.what());
			}
		}

	}
	catch (const HamsterAPI::HamsterError & connection_error)
	{
		HamsterAPI::Log::i("Main", connection_error.what());
	}

	return 0;
}
