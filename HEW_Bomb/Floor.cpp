#include "Floor.h"

Floor::Floor(const GridPosition& position)
    : GridObject(position)
    , m_explosionCount(0)
{
}

Floor::~Floor()
{
}

// ”š”­‚ðŽó‚¯‚½Žž‚Ìˆ—
void Floor::OnExplosion()
{
    // °‚Ì—Ž‰ºˆ—‚ÍŒã‚Å’Ç‰Á‚·‚é
    m_explosionCount++;
}

// ŠK”‚É‰ž‚¶‚½”j‰ó”»’è
bool Floor::IsDestroyed() const
{
    // 1ŠKiYÀ•W‚ª0j‚Ì°‚ÍA‰½“x”š”­‚µ‚Ä‚à”j‰ó‚³‚ê‚È‚¢i–³Ž‹‚·‚éj
    if (GetPosition().GetY() == 0)
    {
        return false;
    }

    // 2ŠKiYÀ•W‚ª‚P)ˆÈã‚Ì°‚ÍA”š”­‚ð1‰ñŽó‚¯‚½‚ç—Ž‰º‚·‚é
    return m_explosionCount >= 1;
    // return false;
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