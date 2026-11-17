#pragma once
#pragma once
#ifndef __UNBREAKOBJECT_H__
#define __UNBREAKOBJECT_H__

#include "GridObject.h"

class UnbreakObject : public GridObject
{
public:
    UnbreakObject(const GridPosition& position);
    ~UnbreakObject() override;

    void OnExplosion() override;
    GridObjectType GetType() const override;
};

#endif
