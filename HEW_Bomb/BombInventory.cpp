#include "BombInventory.h"

#include <limits>

// コンストラクタ
BombInventory::BombInventory()
    : m_counts{}
    , m_selectedType(BombType::None)
{
}

// デストラクタ
BombInventory::~BombInventory()
{
}

// 爆弾の種類が有効か確認する
bool BombInventory::IsValidType(BombType type)
{
    switch (type)
    {
    case BombType::Cross:
    case BombType::Cylinder:
    case BombType::WideCube:
    case BombType::VShape:
    case BombType::Cube:
    case BombType::Circle:
    case BombType::InvertedPyramid:
        return true;

    default:
        return false;
    }
}

// 爆弾の種類を配列の番号に変換する
std::size_t BombInventory::GetIndex(BombType type)
{
    return static_cast<std::size_t>(type);
}

// 爆弾の所持数を設定する
bool BombInventory::SetCount(BombType type, int count)
{
    if (!IsValidType(type) || count < 0)
    {
        return false;
    }

    m_counts[GetIndex(type)] = count;

    return true;
}

// 爆弾の所持数を取得する
int BombInventory::GetCount(BombType type) const
{
    if (!IsValidType(type))
    {
        return 0;
    }

    return m_counts[GetIndex(type)];
}

// 爆弾を選択する
bool BombInventory::SelectBomb(BombType type)
{
    if (!IsValidType(type))
    {
        return false;
    }

    // 所持数が0なら選択できない
    if (GetCount(type) <= 0)
    {
        return false;
    }

    m_selectedType = type;

    return true;
}

// 選択中の爆弾を取得する
BombType BombInventory::GetSelectedType() const
{
    return m_selectedType;
}

// 選択中の爆弾が使用可能か
bool BombInventory::HasSelectedBomb() const
{
    if (!IsValidType(m_selectedType))
    {
        return false;
    }

    return GetCount(m_selectedType) > 0;
}

// 選択中の爆弾を1つ消費する
bool BombInventory::UseSelectedBomb()
{
    if (!HasSelectedBomb())
    {
        return false;
    }

    --m_counts[GetIndex(m_selectedType)];

    return true;
}

// 所持数を追加する
bool BombInventory::AddCount(BombType type, int amount)
{
    if (!IsValidType(type) || amount <= 0)
    {
        return false;
    }

    std::size_t index = GetIndex(type);

    // 整数の上限を超えないようにする
    if (m_counts[index] >
        std::numeric_limits<int>::max() - amount)
    {
        return false;
    }

    m_counts[index] += amount;

    return true;
}

// インベントリを初期化する
void BombInventory::Clear()
{
    m_counts.fill(0);
    m_selectedType = BombType::None;
}