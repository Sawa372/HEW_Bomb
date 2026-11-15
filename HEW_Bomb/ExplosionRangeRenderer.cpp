#include "ExplosionRangeRenderer.h"

#include "Bomb.h"
#include "GridManager.h"
#include "GridCell.h"
#include "Map.h"
#include "Geometory.h"
#include "Explosion.h"

#include <algorithm>
#include <array>
#include <set>
#include <utility>

//==================================================
// コンストラクタ

ExplosionRangeRenderer::ExplosionRangeRenderer()
{
}

//==================================================
// デストラクタ

ExplosionRangeRenderer::~ExplosionRangeRenderer()
{
    Clear();
}


//==================================================
// キャッシュの削除

void ExplosionRangeRenderer::Clear()
{
    m_explosionRangeCaches.clear();
    ClearPreview();
}

//==================================================
// 設置前の爆発範囲を設定する

void ExplosionRangeRenderer::SetPreview(
    BombType type,
    const GridPosition& position,
    const DirectX::XMFLOAT3& rotation)
{
    // 爆弾の種類が指定されていない場合は非表示
    if (type == BombType::None)
    {
        ClearPreview();
        return;
    }

    // 設定内容が変化したか確認
    bool isChanged =
        !m_previewCache.isVisible ||
        m_previewCache.type != type ||
        m_previewCache.position != position ||
        m_previewCache.rotation.x != rotation.x ||
        m_previewCache.rotation.y != rotation.y ||
        m_previewCache.rotation.z != rotation.z;

    // プレビュー情報を設定
    m_previewCache.type = type;
    m_previewCache.position = position;
    m_previewCache.rotation = rotation;
    m_previewCache.isVisible = true;

    // 変更があった場合だけ再計算対象にする
    if (isChanged)
    {
        m_previewCache.isDirty = true;
    }
}

//==================================================
// 設置前の爆発範囲を非表示にする

void ExplosionRangeRenderer::ClearPreview()
{
    m_previewCache.isVisible = false;
    m_previewCache.isDirty = true;

    m_previewCache.affectedPositions.clear();
    m_previewCache.wireframeLines.clear();
}


//==================================================
// 爆発範囲の描画

void ExplosionRangeRenderer::Draw(
    const Map& map,
    const DirectX::XMFLOAT4X4& view,
    const DirectX::XMFLOAT4X4& projection)
{
    // カメラ設定
    Geometory::SetView(view);
    Geometory::SetProjection(projection);

    // 設置済み爆弾のキャッシュを更新
    UpdateCache(map);

    // 設置前のプレビューを更新
    UpdatePreviewCache(map.GetGridManager());

    // 設置済み爆弾の範囲を描画
    DrawWireframe();

    // 設置前のプレビューを描画
    DrawPreviewWireframe();
}


//==================================================
// 爆発範囲のキャッシュ更新

void ExplosionRangeRenderer::UpdateCache(const Map& map)
{
    const GridManager& grid = map.GetGridManager();

    // すべてのキャッシュを未使用にする
    for (ExplosionRangeCache& cache : m_explosionRangeCaches)
    {
        cache.isUsed = false;
    }

    // マップ内の爆弾を確認
    const std::vector<GridCell>& cells = grid.GetCells();

    for (const GridCell& cell : cells)
    {
        if (!cell.HasBomb())
        {
            continue;
        }

        // 爆弾を取得
        const Bomb* pBomb =
            map.GetBombAt(cell.GetPosition());

        if (pBomb == nullptr)
        {
            continue;
        }

        // 対応するキャッシュを検索
        auto cacheIt = std::find_if(
            m_explosionRangeCaches.begin(),
            m_explosionRangeCaches.end(),
            [pBomb](const ExplosionRangeCache& cache)
            {
                return cache.pBomb == pBomb;
            }
        );

        //==================================================
        // キャッシュが存在しない場合

        if (cacheIt == m_explosionRangeCaches.end())
        {
            ExplosionRangeCache cache{};

            cache.pBomb = pBomb;
            cache.type = pBomb->GetType();
            cache.position = pBomb->GetPosition();
            cache.rotation = pBomb->GetRotation();

            // 爆発範囲を計算
            cache.affectedPositions =
                pBomb->GetAffectedPositions(grid);

            // ワイヤーフレームを作成
            BuildWireframe(cache, grid);

            cache.isUsed = true;

            m_explosionRangeCaches.push_back(
                std::move(cache)
            );
        }

        //==================================================
        // 既存のキャッシュが存在する場合

        else
        {
            const DirectX::XMFLOAT3& rotation =
                pBomb->GetRotation();

            // 爆弾の状態が変化しているか確認
            bool isChanged =
                cacheIt->type != pBomb->GetType() ||
                cacheIt->position != pBomb->GetPosition() ||
                cacheIt->rotation.x != rotation.x ||
                cacheIt->rotation.y != rotation.y ||
                cacheIt->rotation.z != rotation.z;

            // 変更があった場合だけ再計算
            if (isChanged)
            {
                cacheIt->type = pBomb->GetType();
                cacheIt->position = pBomb->GetPosition();
                cacheIt->rotation = rotation;

                // 爆発範囲を再計算
                cacheIt->affectedPositions =
                    pBomb->GetAffectedPositions(grid);

                // ワイヤーフレームも再作成
                BuildWireframe(*cacheIt, grid);
            }

            cacheIt->isUsed = true;
        }
    }

    //==================================================
    // 使用されなくなったキャッシュを削除

    m_explosionRangeCaches.erase(
        std::remove_if(
            m_explosionRangeCaches.begin(),
            m_explosionRangeCaches.end(),
            [](const ExplosionRangeCache& cache)
            {
                return !cache.isUsed;
            }
        ),
        m_explosionRangeCaches.end()
    );
}

//==================================================
// 設置前の爆発範囲キャッシュ更新

void ExplosionRangeRenderer::UpdatePreviewCache(
    const GridManager& grid)
{
    // 非表示、または変更がなければ何もしない
    if (!m_previewCache.isVisible ||
        !m_previewCache.isDirty)
    {
        return;
    }

    // プレビュー用のExplosionを作成
    Explosion explosion;

    explosion.SetType(m_previewCache.type);
    explosion.SetAnchor(m_previewCache.position);
    explosion.SetRotation(m_previewCache.rotation);

    // 爆発対象のマスを計算
    ExplosionRangeCache cache{};

    cache.affectedPositions =
        explosion.GetAffectedPositions(grid);

    // ワイヤーフレームを作成
    BuildWireframe(cache, grid);

    // 計算結果をプレビュー用キャッシュに保存
    m_previewCache.affectedPositions =
        std::move(cache.affectedPositions);

    m_previewCache.wireframeLines =
        std::move(cache.wireframeLines);

    // 更新完了
    m_previewCache.isDirty = false;
}


//==================================================
// ワイヤーフレームの作成

void ExplosionRangeRenderer::BuildWireframe(
    ExplosionRangeCache& cache,
    const GridManager& grid)
{
    // 古い線を削除
    cache.wireframeLines.clear();

    // 同じ線が重複しないように管理
    std::set<std::array<int, 6>> uniqueEdges;

    // 1マスの半分のサイズ
    // 現在のGridManagerはセルサイズ1.0を使用
    const float halfSize = 0.5f;

    // 立方体の12本の辺
    static const int edges[12][2] =
    {
        { 0, 1 }, { 1, 2 }, { 2, 3 }, { 3, 0 },
        { 4, 5 }, { 5, 6 }, { 6, 7 }, { 7, 4 },
        { 0, 4 }, { 1, 5 }, { 2, 6 }, { 3, 7 }
    };

    // 爆発対象のマスを確認
    for (const GridPosition& position : cache.affectedPositions)
    {
        DirectX::XMFLOAT3 center;

        if (!grid.GridToWorld(position, center))
        {
            continue;
        }

        //==================================================
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

        //==================================================
        // グリッド上での頂点座標
        // 重複する辺を判定するために使用

        const int x = position.GetX() * 2;
        const int y = position.GetY() * 2;
        const int z = position.GetZ() * 2;

        int gridCorners[8][3] =
        {
            { x - 1, y - 1, z - 1 },
            { x + 1, y - 1, z - 1 },
            { x + 1, y + 1, z - 1 },
            { x - 1, y + 1, z - 1 },

            { x - 1, y - 1, z + 1 },
            { x + 1, y - 1, z + 1 },
            { x + 1, y + 1, z + 1 },
            { x - 1, y + 1, z + 1 }
        };

        //==================================================
        // 12本の辺を登録

        for (int i = 0; i < 12; ++i)
        {
            int a = edges[i][0];
            int b = edges[i][1];

            // 辺の両端のグリッド座標を取得
            std::array<int, 6> edgeKey =
            {
                gridCorners[a][0],
                gridCorners[a][1],
                gridCorners[a][2],

                gridCorners[b][0],
                gridCorners[b][1],
                gridCorners[b][2]
            };

            // 始点と終点の順番を統一
            bool reverse =
                edgeKey[0] > edgeKey[3] ||
                (edgeKey[0] == edgeKey[3] && edgeKey[1] > edgeKey[4]) ||
                (edgeKey[0] == edgeKey[3] && edgeKey[1] == edgeKey[4] && edgeKey[2] > edgeKey[5]);

            if (reverse)
            {
                std::swap(edgeKey[0], edgeKey[3]);
                std::swap(edgeKey[1], edgeKey[4]);
                std::swap(edgeKey[2], edgeKey[5]);
            }

            // すでに登録されている辺ならスキップ
            if (!uniqueEdges.insert(edgeKey).second)
            {
                continue;
            }

            // 新しい線を登録
            LineSegment line;

            line.start = corners[a];
            line.end = corners[b];

            cache.wireframeLines.push_back(line);
        }
    }
}


//==================================================
// 設置済み爆弾のワイヤーフレーム描画

void ExplosionRangeRenderer::DrawWireframe()
{
    const DirectX::XMFLOAT4 explosionColor(
        1.0f, 0.1f, 0.0f, 1.0f
    );

    for (const ExplosionRangeCache& cache :
        m_explosionRangeCaches)
    {
        DrawLineSegments(
            cache.wireframeLines,
            explosionColor
        );
    }
}


//==================================================
// 設置前のワイヤーフレーム描画

void ExplosionRangeRenderer::DrawPreviewWireframe()
{
    if (!m_previewCache.isVisible)
    {
        return;
    }

    const DirectX::XMFLOAT4 previewColor(
        0.0f, 0.8f, 1.0f, 1.0f
    );

    DrawLineSegments(
        m_previewCache.wireframeLines,
        previewColor
    );
}


//==================================================
// ワイヤーフレームの共通描画

void ExplosionRangeRenderer::DrawLineSegments(
    const std::vector<LineSegment>& lines,
    const DirectX::XMFLOAT4& color)
{
    const int maxLinesPerDraw = 1000;

    int lineCount = 0;

    for (const LineSegment& line : lines)
    {
        Geometory::AddLine(
            line.start,
            line.end,
            color
        );

        ++lineCount;

        if (lineCount >= maxLinesPerDraw)
        {
            Geometory::DrawLines();
            lineCount = 0;
        }
    }

    if (lineCount > 0)
    {
        Geometory::DrawLines();
    }
}