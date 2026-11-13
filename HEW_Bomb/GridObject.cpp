#include "GridObject.h"

GridObject::GridObject(const GridPosition& position)
    : m_position(position)
    , m_isDestroyed(false)
{
}

GridObject::~GridObject()
{
}

const GridPosition& GridObject::GetPosition() const
{
    return m_position;
}

bool GridObject::IsDestroyed() const
{
    return m_isDestroyed;
}

void GridObject::Destroy()
{
    m_isDestroyed = true;
}