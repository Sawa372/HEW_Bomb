#pragma once
#ifndef __GENERATOR_H__
#define __GENERATOR_H__

#include "GridObject.h"

class Generator : public GridObject
{
public:
    Generator(const GridPosition& position);
    ~Generator() override;

    void OnExplosion() override;
    GridObjectType GetType() const override;

private:
    int m_explosionCount;
};

#endif
