/*
 * @file	Robot.cpp
 *
 * @author	Yuval Goldberg
 * @since	29/06/2017
 */

#include "Robot.h"

#define MOVE_SPEED			   (0.8)
#define TURN_SPEED			   (0.1)
#define MIN_DISTANCE_FROM_WALL (0.8)
#define OBS_RANGE		   	   (10)

Robot::Robot(double size) : m_size(size), m_direction(), currX(0), currY(0), currYaw(0)
{
	directions[eRobotDirection_front] = t_direction(MOVE_SPEED, 0.0);
	directions[eRobotDirection_right] = t_direction(TURN_SPEED, 45.0);
	directions[eRobotDirection_left]  = t_direction(TURN_SPEED, -45.0);
	directions[eRobotDirection_back]  = t_direction((-1) * MOVE_SPEED, 0.0);

	// Initialize the hamster
	try
	{
		if (m_hamster != NULL)
		{
			delete m_hamster;
		}

		m_hamster = new HamsterAPI::Hamster(1);
		SetDirection(eRobotDirection_front);
		sleep(1);
	}
	catch (const HamsterError & connection_error)
	{
		Log::i("Hamster CTOR", connection_error.what());
	}
}

Robot& Robot::GetInstance()
{
	static Robot instance;

	return instance;
}

Pose Robot::GetPosition() const
{
	return m_hamster->getPose();
}

OccupancyGrid Robot::GetOccupancyGrid() const
{
	return m_hamster->getSLAMMap();
}

void Robot::SetSize(double size)
{
	m_size = size;
}

double Robot::GetSize() const
{
	return m_size;
}

bool Robot::IsConnected() const
{
	bool isConnected = false;

	try
	{
		isConnected = m_hamster->isConnected();
	}
	catch (const HamsterError & connection_error)
	{
		Log::i("Hamster connection", connection_error.what());
	}

	return isConnected;
}

void Robot::UpdatePose()
{
	Pose pose = GetPosition();

	currX = pose.getX();
	currY = pose.getY();
	currYaw = pose.getHeading();
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

bool Robot::CheckWallAhead() const
{
	int count = 0;
	std::vector<double> distances;
	GetScansBetween(180 - OBS_RANGE, 180 + OBS_RANGE, distances);//scan the distance from obstacle ahead between Degrees : [170,190]
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

void Robot::GetScansBetween(double min, double max,std::vector<double> & distances) const
{
	HamsterAPI::LidarScan scan = m_hamster->getLidarScan();

	for (size_t i = 0; i < scan.getScanSize(); i++)
	{
		double degree = scan.getScanAngleIncrement() * i;
		if (degree >= min && degree <= max)//check between the 2 degree
			distances.push_back(scan.getDistance(i));
	}
}

void Robot::MoveAround()
{
	if (CheckWallAhead())
	{
		Stop();
		double r = (double) rand() / (double) RAND_MAX;

		if (r < 0.4)
		{
			SetDirection(eRobotDirection_right);
		} else {
			SetDirection(eRobotDirection_left);
		}

		while (CheckWallAhead())
		{
			Move();
		}
	}

	SetDirection(eRobotDirection_front);
	Move();
}

