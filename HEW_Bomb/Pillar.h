#pragma once
#ifndef __PILLAR_H__
#define __PILLAR_H__

#include "GridObject.h"

class Pillar : public GridObject
{
public:
    Pillar(const GridPosition& position);
    ~Pillar() override;

    void OnExplosion() override;
    GridObjectType GetType() const override;
};

#endif
