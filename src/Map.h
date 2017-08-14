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

	int GetWidth();
	int GetHeight();

	void Show();
	OccupancyGrid* GetCoarseGrid();
	ECellType** GetMap();
	cv::Mat* GetCoarseMatrix();

private:
	void InitCell(OccupancyGrid &grid, uint32_t i, uint32_t j);
	void ConvertToCoarseGrid();
	void Draw();

private:
	OccupancyGrid& m_grid;
	OccupancyGrid *m_coarseGrid;
	ECellType** m_map;
	cv::Mat m_matrix;
	int m_robotSizeInPixels;
};

#endif /* MAP_H_ */
