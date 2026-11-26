#pragma once
#ifndef __GRID_OBJECT_MANAGER_H__
#define __GRID_OBJECT_MANAGER_H__

#include <vector>
#include <memory>

#include "GridPosition.h"
#include "GridObjectType.h"

class GridObject;

// グリッド上のオブジェクトを管理するクラス
class GridObjectManager
{
public:
    GridObjectManager();
    ~GridObjectManager();

    // オブジェクトを生成して登録
    bool CreateObject(
        GridObjectType type,
        const GridPosition& position
    );

    // 指定した位置のオブジェクトを取得
    GridObject* GetObjectAt(const GridPosition& position);

    // 指定した位置のオブジェクトを取得（読み取り専用）
    const GridObject* GetObjectAt(const GridPosition& position) const;

    // 管理しているオブジェクトの数を取得
    int GetObjectCount() const;

    // 指定した位置の床オブジェクトを取得
    GridObject* GetFloorAt(const GridPosition& position);

    // 指定した位置の床オブジェクトを取得（読み取り専用）
    const GridObject* GetFloorAt(const GridPosition& position) const;

    void ProcessExplosion(
        const std::vector<GridPosition>& positions);

    // 破壊されたオブジェクトを削除
    void RemoveDestroyedObjects();

    // 全オブジェクトを解放
    void Clear();

    // Undo用の状態を保存
    bool SaveUndoState();

    // 保存した状態に戻す
    bool RestoreUndoState();

    // 保存した状態を破棄
    void DiscardUndoState();


private:
    // オブジェクトの所有・管理
    std::vector<std::unique_ptr<GridObject>> m_objects;

    // Undo用の保存データ
    std::vector<std::unique_ptr<GridObject>> m_savedObjects;

    // 保存データが存在するか
    bool m_hasSavedState;
};

#endif