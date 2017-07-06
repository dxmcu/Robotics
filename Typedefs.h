/*
 * @file	Typedefs.h
 *
 * @author	Yuval Goldberg
 * @since	06/07/2017
 */

#ifndef TYPEDEFS_H_
#define TYPEDEFS_H_

#include "Node.h"

/**
 * Responsible for the path - size and a double dimension array of nodes
 */
typedef struct
{
	Node** computedPath;
	int sizeOfPath;
} Path;

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
} ECellType;

typedef struct
{
	int i, j;
	double x, y;
	double yaw;
	double belief;
} Particle;

#endif /* TYPEDEFS_H_ */
