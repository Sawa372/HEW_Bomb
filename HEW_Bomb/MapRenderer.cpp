#include "MapRenderer.h"
#include "GridManager.h"
#include "Model.h"
#include "GridCell.h"
#include "GridObjectType.h"

#include <ShaderList.h>
#include <Windows.h>

// コンストラクタ
// 各モデルポインタをnullptrで初期化する
MapRenderer::MapRenderer()
    : m_pWallModel(nullptr)
    , m_pDefaultModel(nullptr)
    , m_pTreeModel(nullptr)
    , m_pRockModel(nullptr)
    , m_pBoxModel(nullptr)
    , m_pPillarModel(nullptr)
    , m_pOilDrumModel(nullptr)
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


    // 壁用モデル

    // 壁の描画に使用するモデルを読み込む（仮モデル）
    m_pWallModel = new Model();
    if (!m_pWallModel->Load(
        "Assets/Model/tree_forest.fbx", 0.5f))
    {
        // 読み込みに失敗した場合はエラーメッセージを表示する
        MessageBoxA(
            nullptr,
            "壁モデルの読み込みに失敗しました。\n(tree_forest.fbx)",
            "MapRenderer Error",
            MB_OK | MB_ICONERROR
        );

        // 読み込み済みのモデルを解放する
        Clear();
        return false;
    }

    // Defaultモデル

    // 種類未指定オブジェクト用のモデルを読み込む（仮モデル）
    m_pDefaultModel = new Model();
    if (!m_pDefaultModel->Load(
        "Assets/Model/plantB_forest.fbx", 0.5f))
    {
        MessageBoxA(
            nullptr,
            "Defaultモデルの読み込みに失敗しました。\n(plantB_forest.fbx)",
            "MapRenderer Error",
            MB_OK | MB_ICONERROR
        );

        Clear();
        return false;
    }

    // Treeモデル

    // 木の描画に使用するモデルを読み込む（仮モデル）
    m_pTreeModel = new Model();
    if (!m_pTreeModel->Load(
        "Assets/Model/tree_forest.fbx", 0.5f))
    {
        MessageBoxA(
            nullptr,
            "Treeモデルの読み込みに失敗しました。\n(tree_forest.fbx)",
            "MapRenderer Error",
            MB_OK | MB_ICONERROR
        );

        Clear();
        return false;
    }

    // Rockモデル

    // 岩の描画に使用するモデルを読み込む（仮モデル）
    m_pRockModel = new Model();
    if (!m_pRockModel->Load(
        "Assets/Model/rocksA_desert.fbx", 0.5f))
    {
        MessageBoxA(
            nullptr,
            "Rockモデルの読み込みに失敗しました。\n(rocksA_desert.fbx)",
            "MapRenderer Error",
            MB_OK | MB_ICONERROR
        );

        Clear();
        return false;
    }

    // Boxモデル

    // 箱の描画に使用するモデルを読み込む（仮モデル）
    m_pBoxModel = new Model();
    if (!m_pBoxModel->Load(
        "Assets/Model/flag_teamBlue.fbx", 0.5f))
    {
        MessageBoxA(
            nullptr,
            "Boxモデルの読み込みに失敗しました。\n(flag_teamBlue.fbx)",
            "MapRenderer Error",
            MB_OK | MB_ICONERROR
        );

        Clear();
        return false;
    }

    // Pillarモデル

    // 柱の描画に使用するモデルを読み込む（仮モデル）
    m_pPillarModel = new Model();
    if (!m_pPillarModel->Load(
        "Assets/Model/flag_teamRed.fbx", 0.5f))
    {
        MessageBoxA(
            nullptr,
            "Pillarモデルの読み込みに失敗しました。\n(flag_teamRed.fbx)",
            "MapRenderer Error",
            MB_OK | MB_ICONERROR
        );

        Clear();
        return false;
    }

    // OilDrumモデル

    // オイル缶の描画に使用するモデルを読み込む（仮モデル）
    m_pOilDrumModel = new Model();
    if (!m_pOilDrumModel->Load(
        "Assets/Model/flag_teamYellow.fbx", 0.5f))
    {
        MessageBoxA(
            nullptr,
            "OilDrumモデルの読み込みに失敗しました。\n(flag_teamYellow.fbx)",
            "MapRenderer Error",
            MB_OK | MB_ICONERROR
        );

        Clear();
        return false;
    }

    return true;
}


//==================================================
// モデルの解放

// 読み込んだすべてのモデルを解放する
void MapRenderer::Clear()
{
    // 壁用モデルを解放
    delete m_pWallModel;
    m_pWallModel = nullptr;

    // Defaultモデルを解放
    delete m_pDefaultModel;
    m_pDefaultModel = nullptr;

    // Treeモデルを解放
    delete m_pTreeModel;
    m_pTreeModel = nullptr;

    // Rockモデルを解放
    delete m_pRockModel;
    m_pRockModel = nullptr;

    // Boxモデルを解放
    delete m_pBoxModel;
    m_pBoxModel = nullptr;

    // Pillarモデルを解放
    delete m_pPillarModel;
    m_pPillarModel = nullptr;

    // OilDrumモデルを解放
    delete m_pOilDrumModel;
    m_pOilDrumModel = nullptr;
}


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
        if (!cell.IsWall() && !cell.HasObject() && !cell.HasFloor())
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
            floorPosition.y -= 0.2f;

            // 岩モデルを床として使用
            DrawModel(m_pRockModel, floorPosition);
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
            case GridObjectType::Default:
                pModel = m_pDefaultModel;
                break;

            case GridObjectType::Tree:
                pModel = m_pTreeModel;
                break;

            case GridObjectType::Box:
                pModel = m_pBoxModel;
                break;

            case GridObjectType::Rock:
                pModel = m_pRockModel;
                break;

            case GridObjectType::Pillar:
                pModel = m_pPillarModel;
                break;

            case GridObjectType::OilDrum:
                pModel = m_pOilDrumModel;
                break;

            default:
                // 対応するモデルがない場合はnullptrのままにする
                break;
            }

            // 選択したモデルをセルのワールド座標に描画する
            // モデルがnullptrの場合はDrawModel側で処理を中止する
            DrawModel(pModel, worldPosition);
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