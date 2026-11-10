#include "Map.h"

Map::Map()
    : m_gridManager()
{
}

// マップの初期化
bool Map::Initialize()
{
    // マップの最小座標
    const GridPosition minPosition(-10, 0, -10);

    // マップの最大座標
    const GridPosition maxPosition(10, 0, 10);

    // 1マスのワールドサイズ
    if (!m_gridManager.SetCellSize(1.0f, 1.0f, 1.0f))
    {
        Clear();
        return false;
    }

    // グリッドの原点
    m_gridManager.SetGridOrigin(
        DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f)
    );

    // グリッドを初期化
    if (!m_gridManager.Initialize(minPosition, maxPosition))
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
    //// 壁の配置
    //if (!m_gridManager.SetWall(GridPosition(-5, 0, 3))) return false;
    //if (!m_gridManager.SetWall(GridPosition(5, 0, 4))) return false;
    //if (!m_gridManager.SetWall(GridPosition(5, 0, 5))) return false;

    //// オブジェクトの配置
    //if (!m_gridManager.SetObject(GridPosition(3, 0, 3))) return false;
    //if (!m_gridManager.SetObject(GridPosition(4, 0, 3))) return false;
    //if (!m_gridManager.SetObject(GridPosition(8, 0, 8))) return false;

    //// プレイヤーの配置
    //if (!m_gridManager.SetPlayer(GridPosition(0, 0, 0))) return false;

    // オブジェクトの配置
    if (!m_gridManager.SetObject(
        GridPosition(3, 0, 3),
        GridObjectType::Tree))
    {
        return false;
    }

    if (!m_gridManager.SetObject(
        GridPosition(4, 0, 3),
        GridObjectType::Rock))
    {
        return false;
    }

    if (!m_gridManager.SetObject(
        GridPosition(8, 0, 8),
        GridObjectType::Tree))
    {
        return false;
    }

    if (!m_gridManager.SetObject(
        GridPosition(2, 0, 3),
        GridObjectType::Box))
    {
        return false;
    }

    if (!m_gridManager.SetObject(
        GridPosition(6, 0, 3),
        GridObjectType::Pillar))
    {
        return false;
    }

    if (!m_gridManager.SetObject(
        GridPosition(7, 0, 4),
        GridObjectType::OilDrum))
    {
        return false;
    }

    return true;
}