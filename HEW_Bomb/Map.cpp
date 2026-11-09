#include "Map.h"

Map::Map()
    : m_gridManager()
{
}

// マップの初期化
bool Map::Initialize(int width, int height, int depth)
{
    // グリッドを初期化
    if (!m_gridManager.Initialize(width, height, depth))
    {
        return false;
    }

    // ステージを作成
    if (!CreateStage())
    {
        Clear();
        return false;
    }

    return true;
}

// マップの解放
void Map::Clear()
{
    m_gridManager.Clear();
}

// グリッドマネージャーの取得
GridManager& Map::GetGridManager()
{
    return m_gridManager;
}

const GridManager& Map::GetGridManager() const
{
    return m_gridManager;
}

// ステージの配置を作成
bool Map::CreateStage()
{
    // 壁の配置
    if (!m_gridManager.SetWall(GridPosition(5, 0, 3))) return false;
    if (!m_gridManager.SetWall(GridPosition(5, 0, 4))) return false;
    if (!m_gridManager.SetWall(GridPosition(5, 0, 5))) return false;

    // オブジェクトの配置
    if (!m_gridManager.SetObject(GridPosition(3, 0, 3))) return false;
    if (!m_gridManager.SetObject(GridPosition(4, 0, 3))) return false;
    if (!m_gridManager.SetObject(GridPosition(8, 0, 8))) return false;

    return true;
}