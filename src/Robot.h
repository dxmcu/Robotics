/*
 * @file	Robot.h
 *
 * @author	Yuval Goldberg
 * @since	29/06/2017
 */

#ifndef ROBOT_H_
#define ROBOT_H_

#include <stdio.h>
#include <HamsterAPIClientCPP/Hamster.h>
#include "LocalizationManager.h"

using namespace HamsterAPI;

#define DEFAULT_ROBOT_SIZE	0.25

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
	Robot(double x = 0, double y = 0, double yaw = 0, double size = DEFAULT_ROBOT_SIZE);
	virtual ~Robot() {};

	static Robot& GetInstance();
	bool IsConnected() const;
	Pose GetPosition() const;
	void SetSize(double size);
	double GetSize() const;
	OccupancyGrid GetOccupancyGrid() const;
	void UpdatePose();
	void SetInitPose(double x, double y, double yaw);

	void Update(LocalizationManager* locManager);

	/* Movement */
	void MoveAround();
	void Stop();
	void Move();
	void SetDirection(ERobotDirection direction);
	void SetSpeed(double speed);
	void SetAngle(double angle);

	bool CheckWallAhead() const;
	void GetScansBetween(double min, double max,std::vector<double> & distances) const;

private:
	Hamster* m_hamster;

	double m_size;

	t_direction m_direction;

	t_direction directions[eRobotDirection_numberOfDirections];

	double currX, currY, currYaw;
	double lastX, lastY, lastYaw;
};

#endif /* ROBOT_H_ */
