/*
 * @file	Typedefs.h
 *
 * @author	Yuval Goldberg
 * @since	06/07/2017
 */

#ifndef TYPEDEFS_H_
#define TYPEDEFS_H_

#include <stdint.h>
#include <HamsterAPIClientCPP/Hamster.h>
#include "Node.h"

/**
 * Responsible for the path - size and a double dimension array of nodes
 */
typedef struct
{
	Node** computedPath;
	uint32_t length;
} Path;

typedef struct point
{
	double x;
	double y;
	double yaw;

	point(double x = 0, double y = 0, double yaw = 0)
	{
		this->x = x;
		this->y = y;
		this->yaw = yaw;
	}
} Position;

/**
 * All the available cell types
 */
typedef enum
{
	eCellType_emptyCell = 0,
	eCellType_occupiedCell,
	eCellType_nearOccupied,
	eCellType_startCell,
	eCellType_endCell,
	eCellType_pathCell,
	eCellType_wayPointCell,
	eCellType_particleCell,
} ECellType;

const cv::Vec3b RED_COLOR = cv::Vec3b(0,0,255);
const cv::Vec3b BLUE_COLOR = cv::Vec3b(255,0,0);
const cv::Vec3b GREEN_COLOR = cv::Vec3b(0,255,0);
const cv::Vec3b YELLOW_COLOR = cv::Vec3b(0,255,255);
const cv::Vec3b WHITE_COLOR = cv::Vec3b(255,255,255);
const cv::Vec3b BLACK_COLOR = cv::Vec3b(0,0,0);
const cv::Vec3b GRAY_COLOR = cv::Vec3b(128,128,128);

const int NUMBER_OF_POSSIBLES_DIRECTIONS = 8;
const int X_DIRECTIONS[NUMBER_OF_POSSIBLES_DIRECTIONS] = { 1, 1, 0, -1, -1, -1, 0, 1 };
const int Y_DIRECTIONS[NUMBER_OF_POSSIBLES_DIRECTIONS] = { 0, 1, 1, 1, 0, -1, -1, -1 };

#endif /* TYPEDEFS_H_ */
