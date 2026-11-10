#include "GridCell.h"

GridCell::GridCell()
    : m_position()
    , m_isWall(false)
    , m_objectType(GridObjectType::None)
    , m_hasBomb(false)
    , m_hasPlayer(false)
{
}

GridCell::GridCell(const GridPosition& position)
    : m_position(position)
    , m_isWall(false)
    , m_objectType(GridObjectType::None)
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
    return m_objectType != GridObjectType::None;
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
    if (object)
    {
        m_objectType = GridObjectType::Default;
    }
    else
    {
        m_objectType = GridObjectType::None;
    }
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

    if (HasObject())
    {
        return false;
    }

    if (m_hasBomb)
    {
        return false;
    }

    return true;
}

GridObjectType GridCell::GetObjectType() const
{
    return m_objectType;
}

void GridCell::SetObjectType(GridObjectType type)
{
    m_objectType = type;
}