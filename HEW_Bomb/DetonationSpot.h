#pragma once
#ifndef __DETONATION_SPOT_H__
#define __DETONATION_SPOT_H__

#include "GridPosition.h"

class DetonationSpot
{
public:
    explicit DetonationSpot(const GridPosition& position);

    const GridPosition& GetPosition() const;

private:
    GridPosition m_position;
};

#endif