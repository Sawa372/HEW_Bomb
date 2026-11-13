#include "MapRenderer.h"
#include "GridManager.h"
#include "Model.h"
#include "GridCell.h"
#include "GridObjectType.h"

#include <ShaderList.h>
#include <Windows.h>

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
    if (!m_pWallModel->Load("Assets/Model/t1.fbx"))
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

    return true;
}


//==================================================
// モデルの解放

//==================================================
// マップの描画

// GridManagerからマップ情報を取得し、壁やオブジェクトを描画する
// view：カメラのビュー行列
// projection：カメラのプロジェクション行列
void MapRenderer::Draw(
    const GridManager& grid,
    const DirectX::XMFLOAT4X4& view,
    const DirectX::XMFLOAT4X4& projection)
{
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
        if (!cell.IsWall() && !cell.HasObject() && !cell.HasFloor() && !cell.HasPlayer())
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

            // 岩モデルを床として使用
            DrawModel(m_pFloorModel, floorPosition);
        }

        // 壁の描画

        // セルに壁がある場合は壁用モデルを描画する
        if (cell.IsWall())
        {
            DrawModel(m_pWallModel, worldPosition);
        }

        // オブジェクトの描画

        // セルにオブジェクトがある場合は種類に応じたモデルを描画する
        if (cell.HasObject())
        {
            // 描画に使用するモデルを保持するポインタ
            Model* pModel = nullptr;

            // セルに設定されているオブジェクトの種類を確認する
            switch (cell.GetObjectType())
            {
            case GridObjectType::Wall:
                DrawModel(m_pWallModel, worldPosition);
                break;

            case GridObjectType::NetworkServer:
                DrawModel(m_pNetworkServerModel, worldPosition);
                break;

            case GridObjectType::Generator:
                DrawModel(m_pGeneratorModel, worldPosition);
                break;

            case GridObjectType::Computer:
                DrawModel(m_pComputerModel, worldPosition);
                break;

            case GridObjectType::Pillar:
                DrawModel(m_pPillarModel, worldPosition);
                break;

            case GridObjectType::OilCan:
                DrawModel(m_pOilCanModel, worldPosition);
                break;
            }
            // 選択したモデルをセルのワールド座標に描画する
            // モデルがnullptrの場合はDrawModel側で処理を中止する
            DrawModel(pModel, worldPosition);
        }

        // 4. プレイヤーの描画
        if (cell.HasPlayer())
        {
            DirectX::XMFLOAT3 playerPos = worldPosition;
            playerPos.y += 0.5f;
            DrawModel(m_pPlayerModel, playerPos);
        }
    }
}


//==================================================
// モデルの描画

// 指定したモデルを指定したワールド座標に描画する
// pModel：描画するモデル
// worldPosition：モデルを配置するワールド座標
void MapRenderer::DrawModel(
    Model* pModel,
    const DirectX::XMFLOAT3& worldPosition)
{
    // 描画するモデルが存在しない場合は処理を終了する
    if (pModel == nullptr)
    {
        return;
    }

    // ワールド行列の作成

    // 指定されたワールド座標へモデルを移動させる行列を作成する
    DirectX::XMMATRIX world =
        DirectX::XMMatrixTranslation(
            worldPosition.x,
            worldPosition.y,
            worldPosition.z
        );

    // XMMATRIXをシェーダーに渡す形式に変換する
    DirectX::XMFLOAT4X4 fWorld;
    DirectX::XMStoreFloat4x4(&fWorld, world);

    // シェーダーにモデルのワールド行列を設定する
    ShaderList::SetWorld(fWorld);

    // シェーダーの設定

    // ワールド空間で描画する頂点シェーダーを設定する
    pModel->SetVertexShader(
        ShaderList::GetVS(ShaderList::VS_WORLD)
    );

    // ランバート反射を使用するピクセルシェーダーを設定する
    pModel->SetPixelShader(
        ShaderList::GetPS(ShaderList::PS_LAMBERT)
    );

    // メッシュの描画

    // モデルが持つすべてのメッシュを順番に描画する
    for (int i = 0; i < pModel->GetMeshNum(); ++i)
    {
        // 描画対象のメッシュ情報を取得する
        const Model::Mesh* mesh = pModel->GetMesh(i);

        // メッシュに対応するマテリアル情報を取得する
        Model::Material material =
            *pModel->GetMaterial(mesh->materialID);

        // シェーダーにマテリアル情報を設定する
        ShaderList::SetMaterial(material);

        // 指定したメッシュを描画する
        pModel->Draw(i);
    }
}