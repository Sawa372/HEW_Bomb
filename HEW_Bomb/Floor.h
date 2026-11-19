#pragma once
#ifndef __FLOOR_H__
#define __FLOOR_H__

#include "GridObject.h"

class Floor : public GridObject
{
public:
    Floor(const GridPosition& position);
    ~Floor() override;

    // プレイヤーの位置を設定
    void SetPlayerPosition(const GridPosition& position);

    // 床が破壊されたとき、真下にプレイヤーがいたか取得
    bool IsPlayerBelow() const;

    // 爆発を受けたときの処理
    void OnExplosion() override;

    // オブジェクトの種類を取得
    GridObjectType GetType() const override;

    // オブジェクトの状態をコピーする
    std::unique_ptr<GridObject> Clone() const override;

private:
    // プレイヤーの位置
    GridPosition m_playerPosition;

    // 床の真下にプレイヤーがいるか
    bool m_isPlayerBelow;
};

#endif