/*
 * @file	Map.h
 *
 * @author	Yuval Goldberg
 * @since	06/07/2017
 */

#ifndef MAP_H_
#define MAP_H_

#include <HamsterAPIClientCPP/Hamster.h>
#include "Typedefs.h"

using namespace HamsterAPI;

class Map
{
public:
	Map(OccupancyGrid &grid, double robotSize);
	virtual ~Map();

	void show();
	OccupancyGrid* getCoarseGrid();
	ECellType** getMap();
	cv::Mat* getCoarseMatrix();

private:
	OccupancyGrid& m_grid;
	OccupancyGrid *coarseGrid;
	ECellType** m_map;
	cv::Mat m_matrix;

	int m_robotSizeInPixels;

	void initCell(OccupancyGrid &grid, uint32_t i, uint32_t j);
	void convertToCoarseGrid();
	void draw();
};

#endif /* MAP_H_ */
