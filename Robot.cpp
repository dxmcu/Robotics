/*
 * Robot.cpp
 *
 *  Created on: Jul 2, 2017
 *      Author: user
 */

#include "Robot.h"

#define MOVE_SPEED			   (0.4)
#define TURN_SPEED			   (0.04)
#define MIN_DISTANCE_FROM_WALL (0.35)

Robot::Robot() : m_direction()
{
	directions[eRobotDirection_front] = t_direction(MOVE_SPEED, 0.0);
	directions[eRobotDirection_right] = t_direction(TURN_SPEED, 45.0);
	directions[eRobotDirection_left]  = t_direction(TURN_SPEED, -45.0);
	directions[eRobotDirection_back]  = t_direction((-1) * MOVE_SPEED, 0.0);

	// Initialize the hamster
	try
	{
		m_hamster = new HamsterAPI::Hamster(1);
		sleep(1);
	}
	catch (const HamsterAPI::HamsterError & connection_error)
	{
		HamsterAPI::Log::i("Hamster CTOR", connection_error.what());
	}
}

Robot& Robot::GetInstance()
{
	static Robot instance;

	return instance;
}

HamsterAPI::Pose Robot::GetPosition()
{
	return m_hamster->getPose();
}

HamsterAPI::OccupancyGrid Robot::GetOccupancyGrid()
{
	return m_hamster->getSLAMMap();
}

bool Robot::IsConnected()
{
	bool isConnected = false;

	try
	{
		isConnected = m_hamster->isConnected();
	}
	catch (const HamsterAPI::HamsterError & connection_error)
	{
		HamsterAPI::Log::i("Hamster connection", connection_error.what());
	}

	return isConnected;
}

/* Movement */

void Robot::SetDirection(ERobotDirection direction)
{
	SetAngle(directions[direction].angle);
	SetSpeed(directions[direction].speed);
}

void Robot::SetSpeed(double speed)
{
	m_direction.speed = speed;
}

void Robot::SetAngle(double angle)
{
	m_direction.angle = angle;
}

void Robot::Move()
{
	m_hamster->sendSpeed(m_direction.speed, m_direction.angle);
}

void Robot::Stop()
{
	m_hamster->sendSpeed(0, 0);
}

bool Robot::checkWallAhead()
{
	int count = 0;
	std::vector<double> distances;
	getScansBetween(m_direction.angle - 10, m_direction.angle + 10, distances);//scan the distance from obstacle ahead between Degrees : [170,190]
	for (size_t i = 0; i <= distances.size(); i++)
	{
		if (distances[i] < MIN_DISTANCE_FROM_WALL)//the distance from the wall ahead
			count++;
	}
	if (count < 4)//if its less then 4 so no wall ahead
		return false;
	else
		HamsterAPI::Log::i("Client", "Close obstacle is in front of me");
	return true;
}

void Robot::getScansBetween(double min, double max,std::vector<double> & distances)
{
	HamsterAPI::LidarScan scan = m_hamster->getLidarScan();

	for (size_t i = 0; i < scan.getScanSize(); i++)
	{
		double degree = scan.getScanAngleIncrement() * i;
		if (degree >= min && degree <= max)//check between the 2 degree
			distances.push_back(scan.getDistance(i));
	}
}

