#pragma once
#include "GridPosition.h"

class GridManager;
class GridCollision;

class Player
{
public:
    Player();
    ~Player();

    void Initialize(const GridPosition& startPos);
    void Update(GridManager& gridManager, const GridCollision& gridCollision);

    GridPosition GetPosition() const { return m_playerPos; }

private:
    GridPosition m_playerPos; // プレイヤーの現在のグリッド座標
    bool m_isKeyReleased;     // キーの連続入力防止用フラグ
};