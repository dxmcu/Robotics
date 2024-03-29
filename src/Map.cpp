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
	ConvertToCoarseGrid();

	printf("===== Map size: %u/%u =====\n", GetHeight(), GetWidth());

	m_matrix = cv::Mat(m_coarseGrid->getHeight(), m_coarseGrid->getWidth(), CV_8UC3);
	m_map = new ECellType*[m_coarseGrid->getHeight()];

	/* Initializing each cell in the grid */
	for (uint32_t row = 0; row < m_coarseGrid->getHeight(); row++)
	{
		m_map[row] = new ECellType[m_coarseGrid->getWidth()];

		for (uint32_t col = 0; col < m_coarseGrid->getWidth(); col++)
		{
			InitCell(*m_coarseGrid, row, col);
			m_map[row][col] = m_coarseGrid->getCell(row,col) == CELL_FREE ? eCellType_emptyCell : eCellType_occupiedCell;
		}
	}

	/* Check all cells for nearbys */
	for (uint32_t row = 0; row < m_coarseGrid->getHeight(); row++)
	{
		for (uint32_t col = 0; col < m_coarseGrid->getWidth(); col++)
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
	delete m_coarseGrid;
}

void Map::InitCell(OccupancyGrid &grid, uint32_t i, uint32_t j)
{
	Cell c = grid.getCell(i, j);

	switch (c)
	{
		case CELL_FREE:
		{
			m_matrix.at<cv::Vec3b>(i, j) = WHITE_COLOR;
			break;
		}
		case CELL_OCCUPIED:
		{
			m_matrix.at<cv::Vec3b>(i, j) = BLACK_COLOR;
			break;
		}
		default:
		{
			m_matrix.at<cv::Vec3b>(i, j) = GRAY_COLOR;
		}
	}
}

int Map::GetWidth()
{
	return m_grid.getWidth() / m_robotSizeInPixels;
}

int Map::GetHeight()
{
	return m_grid.getHeight() / m_robotSizeInPixels;
}

bool Map::IsValidPoint(int row, int col)
{
	return (row >= 0 && row < GetHeight()) &&
		   (col >= 0 && col < GetWidth());
}

void Map::ConvertToCoarseGrid()
{
	int rows = GetHeight();
	int cols = GetWidth();
	double resolution = m_grid.getResolution() * m_robotSizeInPixels;

	m_coarseGrid = new OccupancyGrid(rows, cols, resolution);
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
				m_coarseGrid->setCell(i, j, CELL_OCCUPIED);
			else if (isFree && !isOccupied)
				m_coarseGrid->setCell(i, j, CELL_FREE);
			else
				m_coarseGrid->setCell(i, j, CELL_UNKNOWN);
		}
	}
}

OccupancyGrid* Map::GetCoarseGrid()
{
	return m_coarseGrid;
}

cv::Mat* Map::GetCoarseMatrix()
{
	return &m_matrix;
}

void Map::Draw()
{
	for (uint32_t i = 0; i < m_coarseGrid->getHeight(); i++)
	{
		for (uint32_t j = 0; j < m_coarseGrid->getWidth(); j++)
		{
			switch (m_map[i][j])
			{
				case eCellType_startCell:
				{
					m_matrix.at<cv::Vec3b>(i, j) = GREEN_COLOR;
					break;
				}
				case eCellType_pathCell:
				{
					m_matrix.at<cv::Vec3b>(i, j) = BLUE_COLOR;
					break;
				}
				case eCellType_endCell:
				{
					m_matrix.at<cv::Vec3b>(i, j) = GREEN_COLOR;
					break;
				}
				case eCellType_particleCell:
				{
					m_matrix.at<cv::Vec3b>(i, j) = RED_COLOR;
					break;
				}
				case eCellType_wayPointCell:
				{
					m_matrix.at<cv::Vec3b>(i, j) = YELLOW_COLOR;
					break;
				}
				default: break;
			}
		}
	}
}
void Map::Show()
{
	Draw();
	cv::imshow(CV_MAP_NAME, m_matrix);
	cv::waitKey(1);
}

ECellType** Map::GetMap()
{
	return m_map;
}
