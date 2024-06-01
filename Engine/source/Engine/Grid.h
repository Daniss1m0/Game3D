#pragma once

#include <glm/glm.hpp>
#include <vector>

namespace eng
{
    class Grid
    {
    public:
        Grid(float size, int gridCount);
        void Draw();
        glm::vec3 GetCurrentCellPosition() const;
        void MoveUp();
        void MoveDown();
        void MoveLeft();
        void MoveRight();

    private:
        float m_CellSize;
        int m_GridCount;
        glm::vec3 m_CurrentCellPosition;
        std::vector<glm::vec3> m_CellPositions;
    };
}