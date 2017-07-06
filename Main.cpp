/*
 * @file	Main.cpp
 *
 * @author	Yuval Goldberg
 * @since	29/06/2017
 */

#include <iostream>
#include "Robot.h"
#include "Map.h"

using namespace std;

int main(int argc, char ** argv)
{

	Robot myRobot = Robot::GetInstance();

	sleep(3);

	OccupancyGrid grid = myRobot.GetOccupancyGrid();
	Map* map = new Map(grid, myRobot.GetSize());

	while (myRobot.IsConnected())
	{
		map->show();
//		myRobot.MoveAround();
		sleep(0.5);
	}


	return 0;
}
