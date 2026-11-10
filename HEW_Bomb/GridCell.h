#pragma once
#ifndef __GRID_CELL_H__
#define __GRID_CELL_H__

#include "GridPosition.h"
#include "GridObjectType.h"

//==================================================
// グリッド上の1マスの情報を管理するクラス
// ほぼGridManager専用ヘッダー
// 
// マスの座標や、壁・オブジェクト・爆弾・プレイヤーの
// 設置状態を保持する
//==================================================
class GridCell
{
public:
    // コンストラクタ
    GridCell();
    // デストラクタ
    ~GridCell();

    // 指定されたグリッド座標を持つセルを作成する
    GridCell(const GridPosition& position);

    // マスのグリッド座標を取得する
    const GridPosition& GetPosition() const;

    // マスに壁が設置されているか確認する
    bool IsWall() const;

    // マスにオブジェクトが設置されているか確認する
    bool HasObject() const;

    // 設置されているオブジェクトの種類を取得する
    // オブジェクトがない場合はGridObjectType::Noneを返す
    GridObjectType GetObjectType() const;

    // マスに設置するオブジェクトの種類を設定する
    void SetObjectType(GridObjectType type);

    // マスに爆弾が設置されているか確認する
    bool HasBomb() const;

    // マスにプレイヤーがいるか確認する
    bool HasPlayer() const;

    // マスの壁の有無を設定する
    void SetWall(bool wall);

    // マスのオブジェクトの有無を設定する
    // trueの場合はDefault、falseの場合はNoneに設定する
    void SetObject(bool object);

    // マスの爆弾の有無を設定する
    void SetBomb(bool bomb);

    // マスのプレイヤーの有無を設定する
    void SetPlayer(bool player);

    // マスが移動可能か確認する
    // 壁・オブジェクト・爆弾がある場合は移動不可とする
    bool IsWalkable() const;

private:
    // このセルが存在するグリッド座標
    GridPosition m_position;

    // 壁が設置されているか
    bool m_isWall;

    // 設置されているオブジェクトの種類
    // Noneの場合はオブジェクトが設置されていない
    GridObjectType m_objectType;

    // 爆弾が設置されているか
    bool m_hasBomb;

    // プレイヤーがこのマスにいるか
    bool m_hasPlayer;
};

#endif // __GRID_CELL_H__