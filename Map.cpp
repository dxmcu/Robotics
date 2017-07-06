/*
 * @file	Map.cpp
 *
 * @author	Yuval Goldberg
 * @since	06/07/2017
 */

#include <stdio.h>
#include "Map.h"

#define CV_MAP_NAME				"Hamster_cv_map"

Map::Map(OccupancyGrid &grid, double robotSize) : m_grid(grid)
{
	cv::namedWindow(CV_MAP_NAME, cv::WINDOW_NORMAL);

	m_robotSizeInPixels = (robotSize / 2.0) / (grid.getResolution());
	convertToCoarseGrid();

	m_matrix = cv::Mat(coarseGrid->getHeight(), coarseGrid->getWidth(), CV_8UC3);
	m_map = new ECellType*[coarseGrid->getHeight()];

	/* Initializing each cell in the grid */
	for (uint32_t row = 0; row < coarseGrid->getHeight(); row++)
	{
		m_map[row] = new ECellType[coarseGrid->getWidth()];

		for (uint32_t col = 0; col < coarseGrid->getWidth(); col++)
		{
			initCell(*coarseGrid, row, col);
			m_map[row][col] = coarseGrid->getCell(row,col) == CELL_FREE ? eCellType_emptyCell : eCellType_occupiedCell;
		}
	}

	/* Check all cells for nearbys */
	for (uint32_t row = 0; row < coarseGrid->getHeight(); row++)
	{
		for (uint32_t col = 0; col < coarseGrid->getWidth(); col++)
		{
			if (m_map[row][col] != eCellType_emptyCell)
			{
				continue;
			}

			bool isNearBy = false;

			/* In case the current cell is empty -> check for its nearbys */
			for (int i = -1; i <= 1 && !isNearBy; i++)
			{
				for (int j = -1; j <= 1 && !isNearBy; j++)
				{
					// Don't check the current cell
					if (i == 0 && j == 0)
						continue;

					if (m_map[row + i][col + j] == eCellType_occupiedCell)
					{
						m_map[row][col] = eCellType_nearOccupied;
						isNearBy = true;
					}
				} // j loop
			} // i loop
		} // col loop
	} // row loop
}

Map::~Map()
{
	cv::destroyWindow(CV_MAP_NAME);
	delete coarseGrid;
}

void Map::initCell(OccupancyGrid &grid, uint32_t i, uint32_t j)
{
	Cell c = grid.getCell(i, j);

	switch (c)
	{
		case CELL_FREE:
		{
			m_matrix.at<cv::Vec3b>(i, j)[0] = 255;
			m_matrix.at<cv::Vec3b>(i, j)[1] = 255;
			m_matrix.at<cv::Vec3b>(i, j)[2] = 255;
			break;
		}
		case CELL_OCCUPIED:
		{
			m_matrix.at<cv::Vec3b>(i, j)[0] = 0;
			m_matrix.at<cv::Vec3b>(i, j)[1] = 0;
			m_matrix.at<cv::Vec3b>(i, j)[2] = 0;
			break;
		}
		default:
		{
			m_matrix.at<cv::Vec3b>(i, j)[0] = 128;
			m_matrix.at<cv::Vec3b>(i, j)[1] = 128;
			m_matrix.at<cv::Vec3b>(i, j)[2] = 128;
		}
	}
}

void Map::convertToCoarseGrid()
{
	int rows = m_grid.getHeight() / m_robotSizeInPixels;
	int cols = m_grid.getWidth() / m_robotSizeInPixels;
	double resolution = m_grid.getResolution() * m_robotSizeInPixels;

	coarseGrid = new OccupancyGrid(rows, cols, resolution);
	for (int i  = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			int row = i * m_robotSizeInPixels;
			int col = j * m_robotSizeInPixels;

			bool isOccupied = false;
			bool isFree = false;
			for (int k = row; k < row + m_robotSizeInPixels && !isOccupied; k++)
			{
				for (int m = col; m < col + m_robotSizeInPixels; m++)
				{
					if (m_grid.getCell(k, m) == CELL_FREE)
					{
						isOccupied = false;
						isFree = true;
						break;
					}
					else if (m_grid.getCell(k, m) == CELL_OCCUPIED)
					{
						isOccupied = true;
						isFree = false;
						break;
					}
					else
					{
						isOccupied = false;
						isFree = false;
					}
				}
			}

			if (isOccupied && !isFree)
				coarseGrid->setCell(i, j, CELL_OCCUPIED);
			else if (isFree && !isOccupied)
				coarseGrid->setCell(i, j, CELL_FREE);
			else
				coarseGrid->setCell(i, j, CELL_UNKNOWN);
		}
	}
}

OccupancyGrid* Map::getCoarseGrid()
{
	return coarseGrid;
}

cv::Mat* Map::getCoarseMatrix()
{
	return &m_matrix;
}

void Map::draw()
{
	for (uint32_t i = 0; i < coarseGrid->getHeight(); i++)
	{
		for (uint32_t j = 0; j < coarseGrid->getWidth(); j++)
		{
			switch (m_map[i][j])
			{
				case eCellType_startCell:
				{
					// Green
					m_matrix.at<cv::Vec3b>(i, j)[0] = 0; // BLUE
					m_matrix.at<cv::Vec3b>(i, j)[1] = 255; // GREEN
					m_matrix.at<cv::Vec3b>(i, j)[2] = 0; // RED
					break;
				}
				case eCellType_pathCell:
				{
					// Blue
					m_matrix.at<cv::Vec3b>(i, j)[0] = 255; // BLUE
					m_matrix.at<cv::Vec3b>(i, j)[1] = 0; // GREEN
					m_matrix.at<cv::Vec3b>(i, j)[2] = 0; // RED
					break;
				}
				case eCellType_endCell:
				{
					// Red
					m_matrix.at<cv::Vec3b>(i, j)[0] = 0; // BLUE
					m_matrix.at<cv::Vec3b>(i, j)[1] = 0; // GREEN
					m_matrix.at<cv::Vec3b>(i, j)[2] = 255; // RED
					break;
				}
				case eCellType_wayPointCell:
				{
					// Dark Yellow
					m_matrix.at<cv::Vec3b>(i, j)[0] = 0;
					m_matrix.at<cv::Vec3b>(i, j)[1] = 153;
					m_matrix.at<cv::Vec3b>(i, j)[2] = 153;
					break;
				}
				default: break;
			}
		}
	}
}
void Map::show()
{
	draw();
	cv::imshow(CV_MAP_NAME, m_matrix);
	cv::waitKey(1);
}

ECellType** Map::getMap()
{
	return m_map;
}
