#include "Wall.h"

Wall::Wall(const GridPosition& position)
    : GridObject(position)
{
}

Wall::~Wall()
{
}

void Wall::OnExplosion()
{
    // •Ç‚Í”š”­‚Å‚Í‰ó‚ê‚È‚¢
}

GridObjectType Wall::GetType() const
{
    return GridObjectType::Wall;
}

std::unique_ptr<GridObject> Wall::Clone() const
{
    return std::unique_ptr<GridObject>(
        new Wall(*this)
    );
}