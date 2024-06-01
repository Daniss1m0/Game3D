#include "Grid.h"

namespace eng
{
    Grid::Grid(float cellSize, int gridCount)
        : m_CellSize(cellSize), m_GridCount(gridCount), m_CurrentCellPosition(0.0f, 0.0f, 0.0f)
    {
        for (int i = -gridCount; i <= gridCount; ++i)
        {
            for (int j = -gridCount; j <= gridCount; ++j)
            {
                m_CellPositions.push_back(glm::vec3(i * cellSize, 0.0f, j * cellSize));
            }
        }
    }

    void Grid::Draw()
    {
        // Реализуйте отрисовку сетки
    }

    glm::vec3 Grid::GetCurrentCellPosition() const
    {
        return m_CurrentCellPosition;
    }

    void Grid::MoveUp()
    {
        m_CurrentCellPosition.z -= m_CellSize;
    }

    void Grid::MoveDown()
    {
        m_CurrentCellPosition.z += m_CellSize;
    }

    void Grid::MoveLeft()
    {
        m_CurrentCellPosition.x -= m_CellSize;
    }

    void Grid::MoveRight()
    {
        m_CurrentCellPosition.x += m_CellSize;
    }
}