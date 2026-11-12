#pragma once
#ifndef __GRID_MANAGER_H__
#define __GRID_MANAGER_H__

#include "GridPosition.h"
#include "GridCell.h"
#include <vector>
#include <DirectXMath.h>

//==================================================
// グリッド全体を管理するクラス
// グリッドの作成や各マスの情報管理、
// 壁・オブジェクト・爆弾・プレイヤーの配置や判定を行う
// 基本的には他のクラスからグリッドを操作するときに使用する
//==================================================
class GridManager
{
public:
    // コンストラクタ
    GridManager();

    // デストラクタ
    ~GridManager();


    //==================================================
    // グリッドの初期化・解放

    // 指定した幅・高さ・奥行きでグリッドを作成する
    // 座標の開始位置は(0, 0, 0)となる
    bool Initialize(int width, int height, int depth);

    // 指定した最小座標から最大座標までの範囲でグリッドを作成する
    // マイナス座標を含む範囲にも対応する
    bool Initialize(
        const GridPosition& minPosition,
        const GridPosition& maxPosition
    );

    // グリッドの情報をすべて解放し、初期状態に戻す
    void Clear();


    //==================================================
    // グリッドの情報取得

    // グリッドのX方向のマス数を取得する
    int GetWidth() const;

    // グリッドのY方向のマス数を取得する
    int GetHeight() const;

    // グリッドのZ方向のマス数を取得する
    int GetDepth() const;

    // グリッドの最小座標と最大座標を取得する
    // グリッドが初期化されていない場合はfalseを返す
    bool GetBounds(
        GridPosition& minPosition,
        GridPosition& maxPosition
    ) const;

    // 指定した座標がグリッドの範囲内にあるか確認する
    bool IsInside(const GridPosition& position) const;

    // 指定した座標のマスを取得する
    // 範囲外の場合はnullptrを返す
    // 基本は下をご利用くださいな
    GridCell* GetCell(const GridPosition& position);

    // 指定した座標のマスを取得する（読み取り専用）
    // 範囲外の場合はnullptrを返す　
    // こっちを使ってほしいな
    const GridCell* GetCell(const GridPosition& position) const;

    // グリッド内のすべてのマスを取得する（読み取り専用）
    const std::vector<GridCell>& GetCells() const;


    //==================================================
    // マスの状態確認

    // 指定したマスに壁があるか確認する
    // 範囲外への移動や配置を防ぐため
    // 範囲外は true
    bool IsWall(const GridPosition& position) const;

    // 指定したマスにオブジェクトがあるか確認する
    // 範囲外で false
    bool HasObject(const GridPosition& position) const;

    // 指定したマスに爆弾があるか確認する
    // 範囲外で false
    bool HasBomb(const GridPosition& position) const;

    // 指定したマスにプレイヤーがいるか確認する
    // 範囲外で false
    bool HasPlayer(const GridPosition& position) const;

    // 指定したマスが移動可能か確認する
    // 範囲外で false
    bool CanMove(const GridPosition& position) const;

    // 床があるか
    bool HasFloor(const GridPosition& position) const;


    //==================================================
    // 壁の配置・削除

    // 指定したマスに壁を設置する
    bool SetWall(const GridPosition& position);

    // 指定したマスの壁を削除する
    bool RemoveWall(const GridPosition& position);


    //==================================================
    // オブジェクトの配置・削除

    // 指定したマスにDefaultオブジェクトを設置する
    bool SetObject(const GridPosition& position);

    // 指定した種類のオブジェクトをマスに設置する
    bool SetObject(
        const GridPosition& position,
        GridObjectType type
    );

    // 指定したマスのオブジェクトを削除する
    bool RemoveObject(const GridPosition& position);

    // オブジェクトが配置されているすべての座標を取得する
    std::vector<GridPosition> GetObjectPositions() const;

    // 壁が配置されているすべての座標を取得する
    std::vector<GridPosition> GetWallPositions() const;

    // 床を設置
    bool SetFloor(const GridPosition& position);

    // 床を削除
    bool RemoveFloor(const GridPosition& position);


    //==================================================
    // 爆弾の配置・削除

    // 指定したマスに爆弾を設置する
    bool SetBomb(const GridPosition& position);

    // 指定したマスの爆弾を削除する
    bool RemoveBomb(const GridPosition& position);


    //==================================================
    // プレイヤーの配置・移動

    // 指定したマスにプレイヤーを配置する
    bool SetPlayer(const GridPosition& position);

    // 指定したマスのプレイヤーを削除する
    bool RemovePlayer(const GridPosition& position);

    // プレイヤーを現在のマスから次のマスへ移動させる
    bool MovePlayer(
        const GridPosition& currentPosition,
        const GridPosition& nextPosition
    );


    //==================================================
    // 配置可能判定

    // 指定したマスに爆弾を設置できるか確認する
    bool CanPlaceBomb(const GridPosition& position) const;

    // 指定したマスに壁を設置できるか確認する
    bool CanPlaceWall(const GridPosition& position) const;

    // 指定したマスにオブジェクトを設置できるか確認する
    bool CanPlaceObject(const GridPosition& position) const;

    // 指定したマスにプレイヤーを配置できるか確認する
    bool CanPlacePlayer(const GridPosition& position) const;


    //==================================================
    // ワールド座標との変換設定

    // 1マスあたりのワールド空間でのサイズを設定する
    bool SetCellSize(float x, float y, float z);

    // グリッドの原点となるワールド座標を設定する
    void SetGridOrigin(const DirectX::XMFLOAT3& origin);

    // グリッド座標をワールド座標に変換する
    bool GridToWorld(
        const GridPosition& position,
        DirectX::XMFLOAT3& worldPosition
    ) const;

    // ワールド座標を最も近いグリッド座標に変換する
    // 変換先がグリッドの範囲外の場合はfalseを返す
    bool WorldToGrid(
        const DirectX::XMFLOAT3& worldPosition,
        GridPosition& gridPosition
    ) const;


private:
    // 内部処理

    // グリッド座標を配列のインデックスに変換する
    int GetIndex(const GridPosition& position) const;


private:

    // グリッドの最小座標
    GridPosition m_minPosition;

    // グリッドのX方向のマス数
    int m_width;

    // グリッドのY方向のマス数
    int m_height;

    // グリッドのZ方向のマス数
    int m_depth;

    // グリッド内のすべてのマスの情報を保持する配列
    std::vector<GridCell> m_cells;

    // 1マスあたりのワールド空間でのサイズ
    DirectX::XMFLOAT3 m_cellSize;

    // グリッドの原点となるワールド座標
    DirectX::XMFLOAT3 m_gridOrigin;
};

#endif // __GRID_MANAGER_H__