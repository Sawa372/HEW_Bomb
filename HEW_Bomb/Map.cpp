#include "Map.h"
#include "GridObject.h"
#include "GridObjectType.h"

// コンストラクタ
// GridManagerを初期化する
Map::Map()
    : m_gridManager()
    , m_gridObjectManager()
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
    // オブジェクトを解放
    m_gridObjectManager.Clear();

    // グリッドを解放
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
    // グリッドの範囲を取得
    GridPosition minPosition;
    GridPosition maxPosition;

    if (!m_gridManager.GetBounds(minPosition, maxPosition))
    {
        return false;
    }

    // マップ全体に床を設置
    for (int y = minPosition.GetY(); y <= maxPosition.GetY(); ++y)
    {
        for (int z = minPosition.GetZ(); z <= maxPosition.GetZ(); ++z)
        {
            for (int x = minPosition.GetX(); x <= maxPosition.GetX(); ++x)
            {
                if (!RegisterStageObject(
                    GridObjectType::Floor,
                    GridPosition(x, y, z)))
                {
                    return false;
                }
            }
        }
    }


    //==================================================
 // オブジェクトの配置
 // これは3Dモデルの表示確認用の仮配置

 // Wall
    if (!RegisterStageObject(
        GridObjectType::Wall,
        GridPosition(3, 0, 3)))
    {
        return false;
    }

    // Generator
    if (!RegisterStageObject(
        GridObjectType::Generator,
        GridPosition(4, 0, 3)))
    {
        return false;
    }

    // Wall
    if (!RegisterStageObject(
        GridObjectType::OilCan,
        GridPosition(8, 0, 8)))
    {
        return false;
    }

    // NetworkServer
    if (!RegisterStageObject(
        GridObjectType::NetworkServer,
        GridPosition(2, 0, 3)))
    {
        return false;
    }

    // Pillar
    if (!RegisterStageObject(
        GridObjectType::Pillar,
        GridPosition(6, 0, 3)))
    {
        return false;
    }

    // Generator
    if (!RegisterStageObject(
        GridObjectType::Generator,
        GridPosition(7, 0, 4)))
    {
        return false;
    }

    GridPosition playerStartPos(0, 0, 0);
    if (!m_gridManager.SetPlayer(playerStartPos))
    {
        // 配置に失敗した場合（壁やオブジェクトと被っているなど）
        return false;
    }
    return true;
}

bool Map::RegisterStageObject(GridObjectType type, const GridPosition& position)
{
    // 種類に応じてGridManagerに登録
    switch (type)
    {
    case GridObjectType::Floor:
        if (!m_gridManager.SetFloor(position))
        {
            return false;
        }
        break;

    case GridObjectType::Wall:
        if (!m_gridManager.SetWall(position))
        {
            return false;
        }
        break;

    default:
        if (!m_gridManager.SetObject(position, type))
        {
            return false;
        }
        break;
    }

    // 実際のオブジェクトを生成して登録
    if (!m_gridObjectManager.CreateObject(type, position))
    {
        return false;
    }

    return true;
}

// 爆発範囲内のオブジェクトを処理する
void Map::ProcessExplosion(
    const std::vector<GridPosition>& positions)
{
    // 各オブジェクトに爆発を伝える
    m_gridObjectManager.ProcessExplosion(positions);

    // 破壊されたオブジェクトをグリッドから削除する
    for (const GridPosition& position : positions)
    {
        // 通常オブジェクトを確認
        GridObject* object =
            m_gridObjectManager.GetObjectAt(position);

        if (object != nullptr && object->IsDestroyed())
        {
            if (object->GetType() == GridObjectType::Wall)
            {
                m_gridManager.RemoveWall(position);
            }
            else
            {
                m_gridManager.RemoveObject(position);
            }
        }

        // 床を確認
        GridObject* floor =
            m_gridObjectManager.GetFloorAt(position);

        if (floor != nullptr && floor->IsDestroyed())
        {
            m_gridManager.RemoveFloor(position);
        }
    }

    // 破壊されたオブジェクトを管理リストから削除
    m_gridObjectManager.RemoveDestroyedObjects();
}