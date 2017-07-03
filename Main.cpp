/*
 * Main.cpp
 *
 *  Created on: Jul 2, 2017
 *      Author: user
 */

#include "Robot.h"
#include <iostream>
using namespace std;



int main(int argc, char ** argv)
{
	Robot myRobot = Robot::GetInstance();

	while (myRobot.IsConnected())
	{
		myRobot.Move();
		sleep(0.5);
	}


	return 0;
}
