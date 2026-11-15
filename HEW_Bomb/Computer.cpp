#include "Computer.h"

Computer::Computer(const GridPosition& position)
    : GridObject(position)
{
}

Computer::~Computer()
{
}

void Computer::OnExplosion()
{
    // コンピューターは爆発を受けると破壊される
    Destroy();
}

GridObjectType Computer::GetType() const
{
    return GridObjectType::Computer;
}