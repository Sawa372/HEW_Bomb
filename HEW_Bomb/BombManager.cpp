#include "BombManager.h"
#include "GridManager.h"
#include "BombInventory.h"

#include <utility>

// コンストラクタ
BombManager::BombManager()
{
}

// デストラクタ
BombManager::~BombManager()
{
    // Bombのメモリを解放する
    // グリッドとの同期解除はClear(GridManager&)で行う
    m_bombs.clear();
}

// 選択中の爆弾を設置して登録する
bool BombManager::AddBomb(
    const GridPosition& position,
    GridManager& grid,
    BombInventory& inventory)
{
    // 使用可能な爆弾が選択されているか
    if (!inventory.HasSelectedBomb())
    {
        return false;
    }

    // 選択中の爆弾の種類を取得
    BombType type = inventory.GetSelectedType();

    // 同じ位置に爆弾がある場合は設置しない
    if (HasBombAt(position))
    {
        return false;
    }

    // Bombを生成して管理リストに登録
    std::unique_ptr<Bomb> bomb(
        new Bomb(type, position)
    );

    m_bombs.push_back(std::move(bomb));

    // グリッドに設置
    if (!m_bombs.back()->PlaceOnGrid(grid))
    {
        m_bombs.pop_back();
        return false;
    }

    // 所持数を1つ消費
    if (!inventory.UseSelectedBomb())
    {
        // 消費に失敗した場合は設置を取り消す
        m_bombs.back()->RemoveFromGrid(grid);
        m_bombs.pop_back();

        return false;
    }

    return true;
}
// 指定位置の爆弾を取得する
Bomb* BombManager::GetBombAt(
    const GridPosition& position)
{
    for (const std::unique_ptr<Bomb>& bomb : m_bombs)
    {
        if (bomb->GetPosition() == position)
        {
            return bomb.get();
        }
    }

    return nullptr;
}

// 指定位置の爆弾を取得する（const版）
const Bomb* BombManager::GetBombAt(
    const GridPosition& position) const
{
    for (const std::unique_ptr<Bomb>& bomb : m_bombs)
    {
        if (bomb->GetPosition() == position)
        {
            return bomb.get();
        }
    }

    return nullptr;
}

// 指定位置に爆弾があるか
bool BombManager::HasBombAt(
    const GridPosition& position) const
{
    return GetBombAt(position) != nullptr;
}

// 指定位置の爆弾を回収して削除する
bool BombManager::RemoveBombAt(
    const GridPosition& position,
    GridManager& grid,
    BombInventory& inventory)
{
    for (auto it = m_bombs.begin();
        it != m_bombs.end();
        ++it)
    {
        if ((*it)->GetPosition() == position)
        {
            // 爆発済みの爆弾は回収できない
            if ((*it)->IsExploded())
            {
                return false;
            }

            BombType type = (*it)->GetType();

            // グリッドから爆弾を回収
            if (!(*it)->RemoveFromGrid(grid))
            {
                return false;
            }

            // インベントリに所持数を戻す
            if (!inventory.AddCount(type))
            {
                // 所持数を戻せなかった場合は設置状態に戻す
                (*it)->PlaceOnGrid(grid);
                return false;
            }

            // 管理リストから削除
            m_bombs.erase(it);

            return true;
        }
    }

    return false;
}
// 爆発済みの爆弾を削除する
void BombManager::RemoveExplodedBombs(
    GridManager& grid)
{
    auto it = m_bombs.begin();

    while (it != m_bombs.end())
    {
        if ((*it)->IsExploded())
        {
            // 設置中ならグリッドから解除
            if ((*it)->IsPlaced())
            {
                if (!(*it)->RemoveFromGrid(grid))
                {
                    ++it;
                    continue;
                }
            }

            // 管理リストから削除
            it = m_bombs.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

// すべての爆弾を回収して削除する
void BombManager::Clear(GridManager& grid)
{
    for (const std::unique_ptr<Bomb>& bomb : m_bombs)
    {
        if (bomb->IsPlaced())
        {
            bomb->RemoveFromGrid(grid);
        }
    }

    m_bombs.clear();
}

// 登録されている爆弾の数
int BombManager::GetBombCount() const
{
    return static_cast<int>(m_bombs.size());
}