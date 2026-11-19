#include "Floor.h"

Floor::Floor(const GridPosition& position)
    : GridObject(position)
    , m_playerPosition(0, 0, 0)
    , m_isPlayerBelow(false)
{
}

Floor::~Floor()
{
}

// 爆発を受けた時の処理
void Floor::OnExplosion()
{
    // 1階（Y座標が0）の床は、何度爆発しても破壊されない（無視する）
    /*if (GetPosition().GetY() <= 0)
    {
        return;
    }*/

    // 床の座標を取得
    int nFloorx = GetPosition().GetX();
    int nFloory = GetPosition().GetY();
    int nFloorz = GetPosition().GetZ();

    if (nFloory <= 0)
    {
        m_isPlayerBelow = false;
        return; 
    }

    // 床の真下にプレイヤーがいるか確認
    m_isPlayerBelow =
        (nFloorx == m_playerPosition.GetX() &&
            nFloorz == m_playerPosition.GetZ());
    
    Destroy();

}

// プレイヤーの位置を設定
void Floor::SetPlayerPosition(const GridPosition& position)
{
    m_playerPosition = position;
}

// 床の真下にプレイヤーがいるか取得
bool Floor::IsPlayerBelow() const
{
    return m_isPlayerBelow;
}

GridObjectType Floor::GetType() const
{
    return GridObjectType::Floor;
}

std::unique_ptr<GridObject> Floor::Clone() const
{
    return std::unique_ptr<GridObject>(
        new Floor(*this)
    );
}