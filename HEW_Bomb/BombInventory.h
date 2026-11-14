#pragma once
#ifndef __BOMB_INVENTORY_H__
#define __BOMB_INVENTORY_H__

#include <array>
#include <cstddef>

#include "BombType.h"

class BombInventory
{
public:
    BombInventory();
    ~BombInventory();

    // 爆弾の所持数を設定する
    bool SetCount(BombType type, int count);

    // 爆弾の所持数を取得する
    int GetCount(BombType type) const;

    // 爆弾を選択する
    bool SelectBomb(BombType type);

    // 選択中の爆弾を取得する
    BombType GetSelectedType() const;

    // 選択中の爆弾が使用可能か
    bool HasSelectedBomb() const;

    // 選択中の爆弾を1つ消費する
    bool UseSelectedBomb();

    // 所持数を追加する（回収などで使用）
    bool AddCount(BombType type, int amount = 1);

    // インベントリを初期化する
    void Clear();

private:
    // 爆弾の種類が有効か確認する
    static bool IsValidType(BombType type);

    // 爆弾の種類を配列の番号に変換する
    static std::size_t GetIndex(BombType type);

private:
    // 爆弾の種類ごとの所持数
    // 0番目はNone用
    std::array<int, 8> m_counts;

    // 現在選択中の爆弾
    BombType m_selectedType;
};

#endif