#pragma once
#ifndef __FLOOR_H__
#define __FLOOR_H__

#include "GridObject.h"

class Floor : public GridObject
{
public:
    Floor(const GridPosition& position);
    ~Floor() override;

    // ”š”­‚ğó‚¯‚½‚Ìˆ—
    void OnExplosion() override;

    // ”j‰ó‚³‚ê‚½‚©iŠK”‚É‚æ‚Á‚Ä”»’è‚ğ•Ï‚¦‚éj
    bool IsDestroyed() const;

    GridObjectType GetType() const override;

private:
    int  m_explosionCount = 0; // ”š”­‚ğó‚¯‚½‰ñ”
};

#endif