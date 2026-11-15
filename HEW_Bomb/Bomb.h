#pragma once
#ifndef __BOMB_H__
#define __BOMB_H__

#include "GridPosition.h"
#include "BombType.h"
#include "Explosion.h"

#include <DirectXMath.h>
#include <vector>

class GridManager;

class Bomb
{
public:
    Bomb(
        BombType type,
        const GridPosition& position
    );

    ~Bomb();

    // 爆弾の種類
    BombType GetType() const;

    // 設置位置
    const GridPosition& GetPosition() const;
    void SetPosition(const GridPosition& position);

    // 回転角度（度数法）
    const DirectX::XMFLOAT3& GetRotation() const;
    void SetRotation(const DirectX::XMFLOAT3& rotation);

    // 回転角度を加算する
    void Rotate(float x, float y, float z);

    // 爆発状態
    bool IsExploded() const;

    // 爆発開始（一度だけ成功する）
    bool TryExplode();

    // グリッドに設置する
    bool PlaceOnGrid(GridManager& grid);//////////////////////////////////////////////////

    // グリッドから回収する
    bool RemoveFromGrid(GridManager& grid);/////////////////////////////////////////////////bombに書き換え。分からん

    // 設置されているか
    bool IsPlaced() const;


    // 爆発範囲のサイズを設定（現在は確認用）
    bool SetExplosionSize(int x, int y, int z);

    // 爆発範囲内のグリッド座標を取得
    std::vector<GridPosition> GetAffectedPositions(
        const GridManager& grid
    ) const;

private:
    BombType m_type;

    GridPosition m_position;

    // X・Y・Z軸の回転角度
    DirectX::XMFLOAT3 m_rotation;

    // 爆発済みかどうか
    bool m_isExploded;

    bool m_isPlaced;


    // 爆発情報
    Explosion m_explosion;
};

#endif