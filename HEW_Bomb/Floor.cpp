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
    // è∞ÇÃóéâ∫èàóùÇÕå„Ç≈í«â¡Ç∑ÇÈ
}

GridObjectType Floor::GetType() const
{
    return GridObjectType::Floor;
}

std::unique_ptr<GridObject> Floor::Clone() const
{
    return std::unique_ptr<GridObject>(
        new Floor(*this)
    );
}