#pragma once
#ifndef __FLOOR_H__
#define __FLOOR_H__

#include "GridObject.h"

class Floor : public GridObject
{
public:
    Floor(const GridPosition& position);
    ~Floor() override;

    void OnExplosion() override;
    GridObjectType GetType() const override;
};

#endif