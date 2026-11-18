#pragma once
#ifndef __NETWORK_SERVER_H__
#define __NETWORK_SERVER_H__

#include "GridObject.h"

class NetworkServer : public GridObject
{
public:
    NetworkServer(const GridPosition& position);
    ~NetworkServer() override;

    // 爆発を受けたときの処理
    void OnExplosion() override;

    // オブジェクトの種類を取得
    GridObjectType GetType() const override;

    // オブジェクトの状態をコピーする
    std::unique_ptr<GridObject> Clone() const override;
};

#endif