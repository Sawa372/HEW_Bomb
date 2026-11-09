#include "GridCell.h"

GridCell::GridCell()
    : m_position()
    , m_isWall(false)
    , m_hasObject(false)
    , m_hasBomb(false)
    , m_hasPlayer(false)
{
}

GridCell::GridCell(const GridPosition& position)
    : m_position(position)
    , m_isWall(false)
    , m_hasObject(false)
    , m_hasBomb(false)
    , m_hasPlayer(false)
{
}

const GridPosition& GridCell::GetPosition() const
{
    return m_position;
}

bool GridCell::IsWall() const
{
    return m_isWall;
}

bool GridCell::HasObject() const
{
    return m_hasObject;
}

bool GridCell::HasBomb() const
{
    return m_hasBomb;
}

bool GridCell::HasPlayer() const
{
    return m_hasPlayer;
}

void GridCell::SetWall(bool wall)
{
    m_isWall = wall;
}

void GridCell::SetObject(bool object)
{
    m_hasObject = object;
}

void GridCell::SetBomb(bool bomb)
{
    m_hasBomb = bomb;
}

void GridCell::SetPlayer(bool player)
{
    m_hasPlayer = player;
}

bool GridCell::IsWalkable() const
{
    if (m_isWall)
    {
        return false;
    }

    if (m_hasObject)
    {
        return false;
    }

    if (m_hasBomb)
    {
        return false;
    }

    return true;
}