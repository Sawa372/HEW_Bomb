#pragma once
#ifndef __EXPLOSION_H__
#define __EXPLOSION_H__

#include <DirectXMath.h>
#include <vector>

#include "GridPosition.h"
#include "BombType.h"

class GridManager;

class Explosion
{
public:
    Explosion();
    ~Explosion();

    // 爆発の種類
    void SetType(BombType type);
    BombType GetType() const;

     // 爆弾が置かれたマス（爆発範囲の基準）
    void SetAnchor(const GridPosition& anchor);
    const GridPosition& GetAnchor() const;

    // 回転角度（度数法）
    void SetRotation(const DirectX::XMFLOAT3& rotation);
    const DirectX::XMFLOAT3& GetRotation() const;

    // 爆発範囲のマス数
    bool SetSize(int x, int y, int z);
    const DirectX::XMINT3& GetSize() const;

    // 爆発範囲内か判定する
    bool IsInsideShape(
        const DirectX::XMFLOAT3& localPosition
    ) const;

    // 爆発範囲内のグリッド座標を取得する
    std::vector<GridPosition> GetAffectedPositions(
        const GridManager& grid
    ) const;


   

private:
    BombType m_type;

    GridPosition m_anchor;

    // X・Y・Z軸の回転角度
    DirectX::XMFLOAT3 m_rotation;

    // 爆発範囲のマス数
    DirectX::XMINT3 m_size;
};

#endif