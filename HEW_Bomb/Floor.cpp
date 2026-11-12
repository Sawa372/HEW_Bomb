#include "Floor.h"

Floor::Floor(const GridPosition& position)
    : GridObject(position)
{
}

Floor::~Floor()
{
}

void Floor::OnExplosion()
{
    // °‚Ì—‰ºˆ—‚ÍŒã‚Å’Ç‰Á‚·‚é
}

GridObjectType Floor::GetType() const
{
    return GridObjectType::Floor;
}