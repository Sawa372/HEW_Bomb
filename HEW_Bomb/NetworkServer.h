#pragma once
#ifndef __NETWORK_SERVER_H__
#define __NETWORK_SERVER_H__

#include "GridObject.h"

class NetworkServer : public GridObject
{
public:
    NetworkServer(const GridPosition& position);
    ~NetworkServer() override;

    void OnExplosion() override;
    GridObjectType GetType() const override;
};

#endif