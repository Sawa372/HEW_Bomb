#pragma once
#ifndef __EXPLOSION_RANGE_RENDERER_H__
#define __EXPLOSION_RANGE_RENDERER_H__

#include <DirectXMath.h>
#include <vector>

#include "GridPosition.h"
#include "BombType.h"

class Bomb;
class GridManager;
class Map;

//==================================================
// 爆発範囲の描画管理クラス
//
// 爆発範囲の計算結果をキャッシュし、
// その範囲をワイヤーフレームなどで描画する。
//
// 将来的には3Dモデルによる描画にも対応する。
//==================================================

class ExplosionRangeRenderer
{
public:
    ExplosionRangeRenderer();
    ~ExplosionRangeRenderer();

    // キャッシュをすべて削除する
    void Clear();

    // 爆発範囲を描画する
    void Draw(
        const Map& map,
        const DirectX::XMFLOAT4X4& view,
        const DirectX::XMFLOAT4X4& projection
    );

    // 設置前の爆発範囲を設定する
    void SetPreview(
        BombType type,
        const GridPosition& position,
        const DirectX::XMFLOAT3& rotation
    );

    // 設置前の爆発範囲を非表示にする
    void ClearPreview();

private:

    //==================================================
    // ワイヤーフレームの線分

    struct LineSegment
    {
        DirectX::XMFLOAT3 start;
        DirectX::XMFLOAT3 end;
    };

    //==================================================
    // 爆発範囲のキャッシュ

    struct ExplosionRangeCache
    {
        // 対象の爆弾
        const Bomb* pBomb;

        // 前回計算した爆弾の情報
        BombType type;
        GridPosition position;
        DirectX::XMFLOAT3 rotation;

        // 爆発対象のマス
        std::vector<GridPosition> affectedPositions;

        // ワイヤーフレーム用の線
        std::vector<LineSegment> wireframeLines;

        // 今回の描画で使用されたか
        bool isUsed;
    };

    //==================================================
    // 設置前の爆発範囲キャッシュ

    struct PreviewCache
    {
        PreviewCache()
            : isVisible(false)
            , isDirty(true)
            , type(BombType::None)
            , position()
            , rotation(0.0f, 0.0f, 0.0f)
        {
        }

        bool isVisible;
        bool isDirty;

        BombType type;
        GridPosition position;
        DirectX::XMFLOAT3 rotation;

        std::vector<GridPosition> affectedPositions;
        std::vector<LineSegment> wireframeLines;
    };

private:

    // 爆発範囲のキャッシュを更新する
    void UpdateCache(const Map& map);

    // ワイヤーフレーム用の線を作成する
    void BuildWireframe(
        ExplosionRangeCache& cache,
        const GridManager& grid
    );

    // ワイヤーフレームを描画する
    void DrawWireframe();

    // プレビューのキャッシュ更新
    void UpdatePreviewCache(const GridManager& grid);

    // プレビューのワイヤーフレーム描画
    void DrawPreviewWireframe();

    // ワイヤーフレームを共通描画する
    void DrawLineSegments(
        const std::vector<LineSegment>& lines,
        const DirectX::XMFLOAT4& color
    );

private:

    // 爆弾ごとの爆発範囲キャッシュ
    std::vector<ExplosionRangeCache> m_explosionRangeCaches;

    // 設置前の爆発範囲キャッシュ
    PreviewCache m_previewCache;
};

#endif