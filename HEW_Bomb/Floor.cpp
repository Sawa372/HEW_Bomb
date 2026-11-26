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
    // 1階（Y座標が0）の床は、何度爆発しても破壊されない（無視する）
    if (GetPosition().GetY() <= 0)
    {
        return;
    }

    // 床の落下処理は後で追加する

    Destroy();
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