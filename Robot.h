/*
 * Robot.h
 *
 *  Created on: Jul 2, 2017
 *      Author: user
 */

#ifndef ROBOT_H_
#define ROBOT_H_

#include <stdio.h>
#include <HamsterAPIClientCPP/Hamster.h>

using namespace HamsterAPI;

class Robot
{
public:
	typedef enum
	{
		eRobotDirection_front = 0,
		eRobotDirection_right,
		eRobotDirection_left,
		eRobotDirection_back,

		eRobotDirection_numberOfDirections
	} ERobotDirection;

	typedef struct direction
	{
		double speed;
		double angle;

		direction() : speed(0), angle(0) {}
		direction(double a, double s) : speed(a), angle(s) {}

	} t_direction;

public:
	Robot();
	virtual ~Robot() {};

	static Robot& GetInstance();

	HamsterAPI::Pose GetPosition();

	HamsterAPI::OccupancyGrid GetOccupancyGrid();

	bool IsConnected();

	/* Movement */

	void Stop();

	void Move();

	void SetDirection(ERobotDirection direction);

	void SetSpeed(double speed);

	void SetAngle(double angle);

	bool checkWallAhead();

	void getScansBetween(double min, double max,std::vector<double> & distances);

private:
	HamsterAPI::Hamster* m_hamster;

	t_direction m_direction;

	t_direction directions[eRobotDirection_numberOfDirections];
};

#endif /* ROBOT_H_ */
