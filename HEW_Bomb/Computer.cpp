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

std::unique_ptr<GridObject> Computer::Clone() const
{
    return std::unique_ptr<GridObject>(
        new Computer(*this)
    );
}