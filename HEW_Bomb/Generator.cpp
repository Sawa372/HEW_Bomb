#include "Generator.h"

Generator::Generator(const GridPosition& position)
    : GridObject(position)
    , m_explosionCount(0)
{
}

Generator::~Generator()
{
}

void Generator::OnExplosion()
{
    // ”š”­‚ðŽó‚¯‚½‰ñ”‚ð‘‚â‚·
    ++m_explosionCount;

    // 2‰ñ”š”­‚ðŽó‚¯‚½‚ç”j‰ó
    if (m_explosionCount >= 2)
    {
        Destroy();
    }
}

GridObjectType Generator::GetType() const
{
    return GridObjectType::Generator;
}