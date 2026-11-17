#include "Map.h"
#include "GridObject.h"
#include "GridObjectType.h"
#include "Bomb.h"

#include <algorithm>

// コンストラクタ
// GridManagerを初期化する
Map::Map()
    : m_gridManager()
    , m_gridObjectManager()
    , m_bombManager()
{
}

// デストラクタ
Map::~Map()
{
    Clear();
}


//==================================================
// マップの初期化

// マップを初期化し、ステージの配置を作成する
bool Map::Initialize()
{
    // マップの最小座標
    const GridPosition minPosition(-10, -1, -10);

    // マップの最大座標
    const GridPosition maxPosition(10, 5, 10);

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
    // 爆弾を回収して管理リストを解放
    m_bombManager.Clear(m_gridManager);

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
    //for (int y = minPosition.GetY(); y <= maxPosition.GetY(); y += maxPosition.GetY() - 1)
    //{
        for (int z = minPosition.GetZ(); z <= maxPosition.GetZ(); ++z)
        {
            for (int x = minPosition.GetX(); x <= maxPosition.GetX(); ++x)
            {
                if (!RegisterStageObject(
                    GridObjectType::Floor,
                    GridPosition(x, -1, z)))
                {
                    return false;
                }
            }
        }
    // }
    // 起爆スポット
    if (!AddDetonationSpot(GridPosition(-5, 0, -5)))
        return false;


    //==================================================
    // オブジェクトの配置
    // これは3Dモデルの表示確認用の仮配置

    // Wall
    if (!RegisterStageObject(
        GridObjectType::Wall,
        GridPosition(1, 0, 1)))
    {
        return false;
    }

    // NetworkServer
    if (!RegisterStageObject(
        GridObjectType::NetworkServer,
        GridPosition(2, 0, -2)))
    {
        return false;
    }

    // Generator
    if (!RegisterStageObject(
        GridObjectType::Generator,
        GridPosition(3, 0, 2)))
    {
        return false;
    }

    // Computer
    if (!RegisterStageObject(
        GridObjectType::Computer,
        GridPosition(4, 0, 0)))
    {
        return false;
    }

    // Pillar
    if (!RegisterStageObject(
        GridObjectType::Pillar,
        GridPosition(5, 0, 1)))
    {
        return false;
    }

    // OilCan
    if (!RegisterStageObject(
        GridObjectType::OilCan,
        GridPosition(7, 0, 1)))
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



//==================================================
// 爆弾の管理

//==================================================
// 回転を指定せずに爆弾を設置する

//bool Map::AddBomb(
//    const GridPosition& position,
//    BombInventory& inventory)
//{
//    return AddBomb(
//        position,
//        DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f),
//        inventory
//    );
//}

//==================================================
// 回転を指定して爆弾を設置する

bool Map::AddBomb(
    const GridPosition& position,
    const DirectX::XMFLOAT3& rotation,
    BombInventory& inventory)
{
    return m_bombManager.AddBomb(
        position,
        rotation,
        m_gridManager,
        inventory
    );
}

// 爆弾を回収する
bool Map::RemoveBombAt(
    const GridPosition& position,
    BombInventory& inventory)
{
    return m_bombManager.RemoveBombAt(
        position,
        m_gridManager,
        inventory
    );
}

// 指定位置の爆弾を取得する
Bomb* Map::GetBombAt(
    const GridPosition& position)
{
    return m_bombManager.GetBombAt(position);
}

const Bomb* Map::GetBombAt(
    const GridPosition& position) const
{
    return m_bombManager.GetBombAt(position);
}

// 指定位置に爆弾があるか
bool Map::HasBombAt(
    const GridPosition& position) const
{
    return m_bombManager.HasBombAt(position);
}

// 設置済み爆弾の数
int Map::GetBombCount() const
{
    return m_bombManager.GetBombCount();
}

//==================================================
// 爆弾の起爆・連鎖爆発
bool Map::DetonateBombAt(const GridPosition& position)
{
    // 最初に起爆する爆弾を取得
    Bomb* pFirstBomb = m_bombManager.GetBombAt(position);

    // 爆弾が存在しない場合
    if (pFirstBomb == nullptr)
    {
        return false;
    }

    // 設置されていない爆弾は起爆できない
    if (!pFirstBomb->IsPlaced())
    {
        return false;
    }

    // すでに爆発している場合
    if (pFirstBomb->IsExploded())
    {
        return false;
    }

    //==================================================
    // 連鎖爆発の待機リスト

    std::vector<GridPosition> explosionQueue;

    // 最初の爆弾を登録
    explosionQueue.push_back(position);

    // 次に処理する位置
    size_t currentIndex = 0;

    //==================================================
    // 連鎖爆発の処理

    while (currentIndex < explosionQueue.size())
    {
        // 次の爆弾の位置を取得
        GridPosition currentPosition =
            explosionQueue[currentIndex];

        ++currentIndex;

        // 爆弾を取得
        Bomb* pBomb =
            m_bombManager.GetBombAt(currentPosition);

        if (pBomb == nullptr)
        {
            continue;
        }

        // 設置されていない爆弾は無視
        if (!pBomb->IsPlaced())
        {
            continue;
        }

        // 二重起爆を防止
        if (!pBomb->TryExplode())
        {
            continue;
        }

        //==================================================
        // 爆発範囲を取得

        std::vector<GridPosition> affectedPositions =
            pBomb->GetAffectedPositions(m_gridManager);

        //==================================================
        // 爆発範囲内の爆弾を連鎖対象に登録

        for (const GridPosition& affectedPosition :
            affectedPositions)
        {
            // 爆弾を取得
            Bomb* pChainBomb =
                m_bombManager.GetBombAt(affectedPosition);

            // 爆弾がなければ無視
            if (pChainBomb == nullptr)
            {
                continue;
            }

            // 設置されていない爆弾は無視
            if (!pChainBomb->IsPlaced())
            {
                continue;
            }

            // すでに爆発している爆弾は無視
            if (pChainBomb->IsExploded())
            {
                continue;
            }

            // すでに待機リストに登録されているか確認
            auto it = std::find(
                explosionQueue.begin(),
                explosionQueue.end(),
                affectedPosition
            );

            // 未登録の場合だけ追加
            if (it == explosionQueue.end())
            {
                explosionQueue.push_back(affectedPosition);
            }
        }

        //==================================================
        // 爆発範囲内のオブジェクトを処理

        ProcessExplosion(affectedPositions);
    }

    //==================================================
    // すべての連鎖爆発が終わったら爆弾を削除

    m_bombManager.RemoveExplodedBombs(m_gridManager);

    return true;
}

// 起爆スポットを登録
bool Map::AddDetonationSpot(const GridPosition& position)
{
    // マップの範囲外なら登録しない
    if (!m_gridManager.IsInside(position))
        return false;

    // 同じ座標への重複登録を防止
    for (const DetonationSpot& spot : m_detonationSpots)
    {
        if (spot.GetPosition() == position)
            return false;
    }

    m_detonationSpots.emplace_back(position);

    return true;
}

// 起爆スポット一覧を取得
const std::vector<DetonationSpot>& Map::GetDetonationSpots() const
{
    return m_detonationSpots;
}