#include "NetworkServer.h"

NetworkServer::NetworkServer(const GridPosition& position)
    : GridObject(position)
{
}

NetworkServer::~NetworkServer()
{
}

void NetworkServer::OnExplosion()
{
    // ネットワークサーバーは爆発を受けると破壊される
    Destroy();
}

GridObjectType NetworkServer::GetType() const
{
    return GridObjectType::NetworkServer;
}