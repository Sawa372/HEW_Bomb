#include "GridPosition.h"

GridPosition::GridPosition()
    : m_x(0)
    , m_y(0)
    , m_z(0)
{
}

GridPosition::GridPosition(int x, int y, int z)
    : m_x(x)
    , m_y(y)
    , m_z(z)
{
}

int GridPosition::GetX() const
{
    return m_x;
}

int GridPosition::GetY() const
{
    return m_y;
}

int GridPosition::GetZ() const
{
    return m_z;
}

void GridPosition::Set(int x, int y, int z)
{
    m_x = x;
    m_y = y;
    m_z = z;
}

GridPosition GridPosition::operator+(const GridPosition& other) const
{
    return GridPosition(
        m_x + other.m_x,
        m_y + other.m_y,
        m_z + other.m_z
    );
}

GridPosition GridPosition::operator-(const GridPosition& other) const
{
    return GridPosition(
        m_x - other.m_x,
        m_y - other.m_y,
        m_z - other.m_z
    );
}

GridPosition& GridPosition::operator+=(const GridPosition& other)
{
    m_x += other.m_x;
    m_y += other.m_y;
    m_z += other.m_z;

    return *this;
}

GridPosition& GridPosition::operator-=(const GridPosition& other)
{
    m_x -= other.m_x;
    m_y -= other.m_y;
    m_z -= other.m_z;

    return *this;
}

bool GridPosition::operator==(const GridPosition& other) const
{
    return m_x == other.m_x
        && m_y == other.m_y
        && m_z == other.m_z;
}

bool GridPosition::operator!=(const GridPosition& other) const
{
    return !(*this == other);
}