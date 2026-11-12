#include "Map.h"

// コンストラクタ
// GridManagerを初期化する
Map::Map()
    : m_gridManager()
{
}

// デストラクタ
Map::~Map()
{
}


//==================================================
// マップの初期化

// マップを初期化し、ステージの配置を作成する
bool Map::Initialize()
{
    // マップの最小座標
    const GridPosition minPosition(-10, 0, -10);

    // マップの最大座標
    const GridPosition maxPosition(10, 0, 10);

    // 1マスあたりのワールド空間でのサイズを設定
    if (!m_gridManager.SetCellSize(1.0f, 1.0f, 1.0f))
    {
        Clear();
        return false;
    }

    // グリッド座標(0, 0, 0)に対応するワールド座標を設定
    m_gridManager.SetGridOrigin(
        DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));

    // 指定した最小座標から最大座標までのグリッドを初期化
    if (!m_gridManager.Initialize(minPosition, maxPosition))
    {
        return false;
    }

    // ステージ内の壁やオブジェクトを配置
    if (!CreateStage())
    {
        Clear();
        return false;
    }

    return true;
}


//==================================================
// マップの解放

// グリッド内のセル情報を解放する
void Map::Clear()
{
    m_gridManager.Clear();
}


//==================================================
// GridManagerの取得

// 管理しているGridManagerを取得する
// グリッドの情報を変更する場合に使用する
GridManager& Map::GetGridManager()
{
    return m_gridManager;
}

// 管理しているGridManagerを取得する（読み取り専用）
// グリッドの情報を参照する場合に使用する
const GridManager& Map::GetGridManager() const
{
    return m_gridManager;
}


//==================================================
// ステージの作成

// ステージ内の壁やオブジェクトを配置する
// 配置に失敗した場合はfalseを返す
bool Map::CreateStage()
{
    //==================================================
    // オブジェクトの配置
    // これは3Dモデルの表示確認用の仮配置

    // 木を配置
    if (!m_gridManager.SetObject(
        GridPosition(3, 0, 3),
        GridObjectType::Tree))
    {
        return false;
    }

    // 岩を配置
    if (!m_gridManager.SetObject(
        GridPosition(4, 0, 3),
        GridObjectType::Rock))
    {
        return false;
    }

    // 木を配置
    if (!m_gridManager.SetObject(
        GridPosition(8, 0, 8),
        GridObjectType::Tree))
    {
        return false;
    }

    // 箱を配置
    if (!m_gridManager.SetObject(
        GridPosition(2, 0, 3),
        GridObjectType::Box))
    {
        return false;
    }

    // 柱を配置
    if (!m_gridManager.SetObject(
        GridPosition(6, 0, 3),
        GridObjectType::Pillar))
    {
        return false;
    }

    // オイル缶を配置
    if (!m_gridManager.SetObject(
        GridPosition(7, 0, 4),
        GridObjectType::OilDrum))
    {
        return false;
    }

    return true;
}