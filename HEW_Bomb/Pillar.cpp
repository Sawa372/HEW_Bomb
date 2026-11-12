#include "Pillar.h"

Pillar::Pillar(const GridPosition& position)
    : GridObject(position)
{
}

Pillar::~Pillar()
{
}

void Pillar::OnExplosion()
{
    // ’Œ‚Í”š”­‚ğó‚¯‚é‚Æ”j‰ó‚³‚ê‚é
    // x‚¦‚Ä‚¢‚é°‚ª—‚¿‚éˆ—‚ÍŒã‚Å’Ç‰Á‚·‚é
    Destroy();
}

GridObjectType Pillar::GetType() const
{
    return GridObjectType::Pillar;
}