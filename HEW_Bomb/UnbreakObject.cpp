#include "UnbreakObject.h"

UnbreakObject::UnbreakObject(const GridPosition& position)
    : GridObject(position)
{
}

UnbreakObject::~UnbreakObject()
{
}

void UnbreakObject::OnExplosion()
{
}

GridObjectType UnbreakObject::GetType() const
{
    return GridObjectType::Unbreak;
}
