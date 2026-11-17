#include "MapRenderer.h"
#include "GridManager.h"
#include "Model.h"
#include "GridCell.h"
#include "GridObjectType.h"
#include "Map.h"
#include "Bomb.h"
#include "Geometory.h"

#include <ShaderList.h>
#include <Windows.h>
#include <algorithm>

// コンストラクタ
MapRenderer::MapRenderer()
    : m_pWallModel(nullptr)
    , m_pNetworkServerModel(nullptr)
    , m_pGeneratorModel(nullptr)
    , m_pComputerModel(nullptr)
    , m_pPillarModel(nullptr)
    , m_pOilCanModel(nullptr)
    , m_pFloorModel(nullptr)
    , m_pPlayerModel(nullptr)
    , m_pCubeBombModel(nullptr)
    , m_pWideCubeBombModel(nullptr)
    , m_pCylinderBombModel(nullptr)
    , m_pVShapeBombModel(nullptr)
    , m_pCircleBombModel(nullptr)
    , m_pCrossBombModel(nullptr)
    , m_pInvertedPyramidBombModel(nullptr)
{
}
// デストラクタ
// 管理しているモデルをすべて解放する
MapRenderer::~MapRenderer()
{
    Clear();
}


//==================================================
// 初期化

// 描画に使用する3Dモデルを読み込む
// 読み込みに失敗した場合は、読み込み済みのモデルを解放してfalseを返す
bool MapRenderer::Initialize()
{
    // 既存のモデルを解放して初期状態に戻す
    Clear();
    m_pWallModel = new Model();
    if (!m_pWallModel->Load("Assets/Model/tree_forest.fbx"))
    {
        MessageBoxA(nullptr, "Wallモデルの読み込みに失敗しました。\ntree_forest.fbx", "Error", MB_OK);
        return false;
    }
    
    m_pNetworkServerModel = new Model();
    if (!m_pNetworkServerModel->Load("Assets/Model/plantB_forest.fbx"))
    {
        MessageBoxA(nullptr, "NetworkServerモデルの読み込みに失敗しました。\nplantB_forest.fbx", "Error", MB_OK);
        return false;
    }

    m_pGeneratorModel = new Model();
    if (!m_pGeneratorModel->Load("Assets/Model/rocksA_desert.fbx"))
    {
        MessageBoxA(nullptr, "Generatorモデルの読み込みに失敗しました。\nrocksA_desert.fbx", "Error", MB_OK);
        return false;
    }

    m_pComputerModel = new Model();
    if (!m_pComputerModel->Load("Assets/Model/flag_teamBlue.fbx"))
    {
        MessageBoxA(nullptr, "Computerモデルの読み込みに失敗しました。\nflag_teamBlue.fbx", "Error", MB_OK);
        return false;
    }

    m_pPillarModel = new Model();
    if (!m_pPillarModel->Load("Assets/Model/flag_teamRed.fbx"))
    {
        MessageBoxA(nullptr, "Pillarモデルの読み込みに失敗しました。\nflag_teamRed.fbx", "Error", MB_OK);
        return false;
    }

    m_pOilCanModel = new Model();
    if (!m_pOilCanModel->Load("Assets/Model/flag_teamYellow.fbx"))
    {
        MessageBoxA(nullptr, "OilCanモデルの読み込みに失敗しました。\nflag_teamYellow.fbx", "Error", MB_OK);
        return false;
    }

    m_pFloorModel = new Model();
    if (!m_pFloorModel->Load("Assets/Model/tileMedium_forest.fbx",0.5))
    {
        MessageBoxA(nullptr, "Floorモデルの読み込みに失敗しました。\ntileMedium_forest.fbx", "Error", MB_OK);
        return false;
    }

    // プレイヤーモデル
    m_pPlayerModel = new Model();
    if (!m_pPlayerModel->Load("Assets/Model/ball.fbx", 0.5f)) // パスは実際のファイル名に合わせる
    {
        // エラー処理
        Clear();
        return false;
    }

    //==================================================
    // 爆弾モデルの読み込み

    if (!LoadBombModel(m_pCubeBombModel, "Assets/Model/bomb_circle/Bom_Cube.fbx") ||               // 仮置き
        !LoadBombModel(m_pWideCubeBombModel, "Assets/Model/bomb_circle/Bom_WideCube.fbx") ||           // 仮置き
        !LoadBombModel(m_pCylinderBombModel, "Assets/Model/bomb_circle/Bom_Cylinder.fbx") ||           // 仮置き
        !LoadBombModel(m_pVShapeBombModel, "Assets/Model/bomb_circle/Bom_VShape.fbx") ||             // 仮置き
        !LoadBombModel(m_pCircleBombModel, "Assets/Model/bomb_circle/Bom_Circle.fbx") ||             // 仮置き
        !LoadBombModel(m_pCrossBombModel, "Assets/Model/bomb_circle/Bom_Cross.fbx") ||              // 仮置き
        !LoadBombModel(m_pInvertedPyramidBombModel, "Assets/Model/bomb_circle/Bom_InvertedPyramid.fbx"))      // 仮置き
    {
        MessageBoxA(
            nullptr,
            "爆弾モデルの読み込みに失敗しました。",
            "Error",
            MB_OK
        );

        Clear();
        return false;
    }

    return true;
}


// 爆弾モデルの読み込み
bool MapRenderer::LoadBombModel(
    Model*& pModel,
    const char* filePath)
{
    pModel = new Model();

    if (!pModel->Load(filePath, 0.5f))
    {
        delete pModel;
        pModel = nullptr;
        return false;
    }

    return true;
}

//==================================================
// 設置前の爆発範囲を設定する

void MapRenderer::SetExplosionPreview(
    BombType type,
    const GridPosition& position,
    const DirectX::XMFLOAT3& rotation)
{
    m_explosionRangeRenderer.SetPreview(
        type,
        position,
        rotation
    );
}

//==================================================
// 設置前の爆発範囲を非表示にする

void MapRenderer::ClearExplosionPreview()
{
    m_explosionRangeRenderer.ClearPreview();
}

//==================================================
// マップの描画

// GridManagerからマップ情報を取得し、壁やオブジェクトを描画する
// view：カメラのビュー行列
// projection：カメラのプロジェクション行列
void MapRenderer::Draw(
    const Map& map,
    const DirectX::XMFLOAT4X4& view,
    const DirectX::XMFLOAT4X4& projection)
{
    // GridManagerを取得
    const GridManager& grid = map.GetGridManager();

    // カメラの設定

    // シェーダーにカメラのビュー行列とプロジェクション行列を設定する
    ShaderList::SetView(view);
    ShaderList::SetProjection(projection);


    // セル情報の取得

    // GridManagerからすべてのセルを取得する（読み取り専用）
    const std::vector<GridCell>& cells = grid.GetCells();

    // グリッド内のすべてのセルを順番に確認する
    for (const GridCell& cell : cells)
    {
        // 壁もオブジェクトもないセルは描画する必要がないためスキップする
        if (!cell.IsWall() && !cell.HasObject() && !cell.HasFloor() && !cell.HasPlayer() && !cell.HasBomb())
        {
            continue;
        }


        // ワールド座標への変換

        DirectX::XMFLOAT3 worldPosition;

        // セルのグリッド座標をワールド座標へ変換する
        if (!grid.GridToWorld(
            cell.GetPosition(),
            worldPosition))
        {
            // 座標変換に失敗した場合は、このセルを描画しない
            continue;
        }

        // 床を描画
        if (cell.HasFloor())
        {
            DirectX::XMFLOAT3 floorPosition = worldPosition;

            // 床を少し下げて配置する
            floorPosition.y -= 0.75f;

            
            DrawModel(m_pFloorModel, floorPosition);
        }

        // 壁の描画

        // セルに壁がある場合は壁用モデルを描画する
        if (cell.IsWall())
        {
            DrawModel(m_pWallModel, worldPosition);
        }

        // オブジェクトの描画
        if (cell.HasObject())
        {
            Model* pModel = nullptr;

            switch (cell.GetObjectType())
            {
            case GridObjectType::Wall:
                pModel = m_pWallModel;
                break;

            case GridObjectType::NetworkServer:
                pModel = m_pNetworkServerModel;
                break;

            case GridObjectType::Generator:
                pModel = m_pGeneratorModel;
                break;

            case GridObjectType::Computer:
                pModel = m_pComputerModel;
                break;

            case GridObjectType::Pillar:
                pModel = m_pPillarModel;
                break;

            case GridObjectType::OilCan:
                pModel = m_pOilCanModel;
                break;
            }

            DrawModel(pModel, worldPosition);
        }

        // 4. プレイヤーの描画
        if (cell.HasPlayer())
        {
            DirectX::XMFLOAT3 playerPos = worldPosition;
            playerPos.y += 0.5f;
            DrawModel(m_pPlayerModel, playerPos);
        }

        //==================================================
        // 爆弾の描画

        if (cell.HasBomb())
        {
            // このマスに設置されている爆弾を取得
            const Bomb* pBomb =
                map.GetBombAt(cell.GetPosition());

            if (pBomb != nullptr)
            {
                Model* pModel = nullptr;

                // 爆弾の種類に応じてモデルを選択
                switch (pBomb->GetType())
                {
                case BombType::Cube:
                    pModel = m_pCubeBombModel;
                    break;

                case BombType::WideCube:
                    pModel = m_pWideCubeBombModel;
                    break;

                case BombType::Cylinder:
                    pModel = m_pCylinderBombModel;
                    break;

                case BombType::VShape:
                    pModel = m_pVShapeBombModel;
                    break;

                case BombType::Circle:
                    pModel = m_pCircleBombModel;
                    break;

                case BombType::Cross:
                    pModel = m_pCrossBombModel;
                    break;

                case BombType::InvertedPyramid:
                    pModel = m_pInvertedPyramidBombModel;
                    break;
                }

                // 爆弾の描画位置
                DirectX::XMFLOAT3 bombPosition = worldPosition;
                
                // 爆弾の回転を取得
                const DirectX::XMFLOAT3& rotation =
                    pBomb->GetRotation();

                // 回転を適用して描画
                DrawModel(
                    pModel,
                    bombPosition,
                    rotation
                );
            }
        }
      
    }
    //==================================================
      // 爆発範囲の描画
      // マップ全体の描画が終わってから一度だけ実行

  // 爆発範囲の描画
    m_explosionRangeRenderer.Draw(
        map,
        view,
        projection
    );

    // 起爆スポットの描画
    DrawDetonationSpots(
        map,
        view,
        projection
    );
}

//==================================================
// 起爆スポットのワイヤー描画

void MapRenderer::DrawDetonationSpots(
    const Map& map,
    const DirectX::XMFLOAT4X4& view,
    const DirectX::XMFLOAT4X4& projection)
{
    const GridManager& grid = map.GetGridManager();

    // 起爆スポット一覧を取得
    const auto& spots = map.GetDetonationSpots();

    // カメラ設定
    Geometory::SetView(view);
    Geometory::SetProjection(projection);

    // ワイヤーの色（緑色）
    const DirectX::XMFLOAT4 color(
        0.0f, 1.0f, 0.2f, 1.0f
    );

    // 1マスの半分のサイズ
    const float halfSize = 0.5f;

    // 立方体の12本の辺
    static const int edges[12][2] =
    {
        { 0, 1 }, { 1, 2 }, { 2, 3 }, { 3, 0 },
        { 4, 5 }, { 5, 6 }, { 6, 7 }, { 7, 4 },
        { 0, 4 }, { 1, 5 }, { 2, 6 }, { 3, 7 }
    };

    bool hasLines = false;

    // 登録されている起爆スポットを確認
    for (const auto& spot : spots)
    {
        DirectX::XMFLOAT3 center;

        // グリッド座標からワールド座標に変換
        if (!grid.GridToWorld(spot.GetPosition(), center))
        {
            continue;
        }

        // 立方体の8頂点
        DirectX::XMFLOAT3 corners[8] =
        {
            { center.x - halfSize, center.y - halfSize, center.z - halfSize },
            { center.x + halfSize, center.y - halfSize, center.z - halfSize },
            { center.x + halfSize, center.y + halfSize, center.z - halfSize },
            { center.x - halfSize, center.y + halfSize, center.z - halfSize },

            { center.x - halfSize, center.y - halfSize, center.z + halfSize },
            { center.x + halfSize, center.y - halfSize, center.z + halfSize },
            { center.x + halfSize, center.y + halfSize, center.z + halfSize },
            { center.x - halfSize, center.y + halfSize, center.z + halfSize }
        };

        // 12本の辺を描画
        for (int i = 0; i < 12; ++i)
        {
            int a = edges[i][0];
            int b = edges[i][1];

            Geometory::AddLine(
                corners[a],
                corners[b],
                color
            );

            hasLines = true;
        }
    }

    // 線が存在する場合だけ描画
    if (hasLines)
    {
        Geometory::DrawLines();
    }
}


//==================================================
// モデルの描画

// 指定したモデルを指定したワールド座標に描画する
// pModel：描画するモデル
// worldPosition：モデルを配置するワールド座標
//==================================================
// モデルの描画（回転なし）

void MapRenderer::DrawModel(
    Model* pModel,
    const DirectX::XMFLOAT3& worldPosition)
{
    // 回転なしで描画
    DirectX::XMFLOAT3 rotation(0.0f, 0.0f, 0.0f);

    DrawModel(
        pModel,
        worldPosition,
        rotation
    );
}


//==================================================
// モデルの描画（回転あり）

void MapRenderer::DrawModel(
    Model* pModel,
    const DirectX::XMFLOAT3& worldPosition,
    const DirectX::XMFLOAT3& rotation)
{
    if (pModel == nullptr)
    {
        return;
    }

    //==================================================
    // 度数法からラジアンに変換

    float pitch = DirectX::XMConvertToRadians(rotation.x);
    float yaw = DirectX::XMConvertToRadians(rotation.y);
    float roll = DirectX::XMConvertToRadians(rotation.z);


    //==================================================
    // ワールド行列の作成

    // 回転行列
    DirectX::XMMATRIX rot =
        DirectX::XMMatrixRotationRollPitchYaw(
            pitch,
            yaw,
            roll
        );

    // 移動行列
    DirectX::XMMATRIX trans =
        DirectX::XMMatrixTranslation(
            worldPosition.x,
            worldPosition.y,
            worldPosition.z
        );

    // 回転してから移動
    DirectX::XMMATRIX world = rot * trans;


    //==================================================
    // ワールド行列の設定

    DirectX::XMFLOAT4X4 fWorld;

    DirectX::XMStoreFloat4x4(
        &fWorld,
        world
    );

    ShaderList::SetWorld(fWorld);


    //==================================================
    // シェーダーの設定

    pModel->SetVertexShader(
        ShaderList::GetVS(ShaderList::VS_WORLD)
    );

    pModel->SetPixelShader(
        ShaderList::GetPS(ShaderList::PS_LAMBERT)
    );


    //==================================================
    // メッシュの描画

    for (int i = 0; i < pModel->GetMeshNum(); ++i)
    {
        const Model::Mesh* mesh = pModel->GetMesh(i);

        Model::Material material =
            *pModel->GetMaterial(mesh->materialID);

        ShaderList::SetMaterial(material);

        pModel->Draw(i);
    }
}