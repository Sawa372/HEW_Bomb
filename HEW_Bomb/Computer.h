#pragma once
#ifndef __COMPUTER_H__
#define __COMPUTER_H__

#include "GridObject.h"

class Computer : public GridObject
{
public:
    Computer(const GridPosition& position);
    ~Computer() override;

    void OnExplosion() override;
    GridObjectType GetType() const override;
};

#endif