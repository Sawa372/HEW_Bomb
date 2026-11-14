#include "Bomb.h"
#include "GridManager.h"

// コンストラクタ
Bomb::Bomb(
    BombType type,
    const GridPosition& position)
    : m_type(type)
    , m_position(position)
    , m_rotation(0.0f, 0.0f, 0.0f)
    , m_isExploded(false)
    , m_isPlaced(false)
{
}

// デストラクタ
Bomb::~Bomb()
{
}

// 爆弾の種類
BombType Bomb::GetType() const
{
    return m_type;
}

// 設置位置
const GridPosition& Bomb::GetPosition() const
{
    return m_position;
}

void Bomb::SetPosition(const GridPosition& position)
{
    m_position = position;
}

// 回転角度
const DirectX::XMFLOAT3& Bomb::GetRotation() const
{
    return m_rotation;
}

void Bomb::SetRotation(const DirectX::XMFLOAT3& rotation)
{
    m_rotation = rotation;
}

// 回転角度を加算する
void Bomb::Rotate(float x, float y, float z)
{
    m_rotation.x += x;
    m_rotation.y += y;
    m_rotation.z += z;
}

// 爆発済みかどうか
bool Bomb::IsExploded() const
{
    return m_isExploded;
}

// 爆発開始（一度だけ成功する）
bool Bomb::TryExplode()
{
    if (m_isExploded)
    {
        return false;
    }

    m_isExploded = true;
    return true;
}

// グリッドに設置する
bool Bomb::PlaceOnGrid(GridManager& grid)
{
    // すでに設置済み、または爆発済みなら設置できない
    if (m_isPlaced || m_isExploded)
    {
        return false;
    }

    // GridManagerに爆弾を登録
    if (!grid.SetBomb(m_position))
    {
        return false;
    }

    m_isPlaced = true;

    return true;
}

// グリッドから回収する
bool Bomb::RemoveFromGrid(GridManager& grid)
{
    // 設置されていなければ回収できない
    if (!m_isPlaced)
    {
        return false;
    }

    // GridManagerから爆弾情報を削除
    if (!grid.RemoveBomb(m_position))
    {
        return false;
    }

    m_isPlaced = false;

    return true;
}

// 設置されているか
bool Bomb::IsPlaced() const
{
    return m_isPlaced;
}