#include "DetonationSpot.h"

// コンストラクタ
DetonationSpot::DetonationSpot(const GridPosition& position)
    : m_position(position)
{
}

// 座標取得
const GridPosition& DetonationSpot::GetPosition() const
{
    return m_position;
}