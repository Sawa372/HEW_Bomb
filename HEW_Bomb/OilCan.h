#pragma once
#ifndef __OIL_CAN_H__
#define __OIL_CAN_H__

#include "GridObject.h"

class OilCan : public GridObject
{
public:
    OilCan(const GridPosition& position);
    ~OilCan() override;

    void OnExplosion() override;
    GridObjectType GetType() const override;
};

#endif