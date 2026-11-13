#include "OilCan.h"

OilCan::OilCan(const GridPosition& position)
    : GridObject(position)
{
}

OilCan::~OilCan()
{
}

void OilCan::OnExplosion()
{
    // ƒIƒCƒ‹ŠÊ‚Í”š”­‚ğó‚¯‚é‚Æ”j‰ó‚³‚ê‚é
    // “Áê‚È”š”­ˆ—‚ÍŒã‚Å’Ç‰Á‚·‚é
    Destroy();
}

GridObjectType OilCan::GetType() const
{
    return GridObjectType::OilCan;
}