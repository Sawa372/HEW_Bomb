#pragma once
#ifndef __GRID_OBJECT_TYPE_H__
#define __GRID_OBJECT_TYPE_H__

//==================================================
// グリッドに配置するオブジェクトの種類
// 現在のモデルはすべて仮置き用
//==================================================
enum class GridObjectType
{
    None,       // オブジェクトなし

    Default,    // 種類未指定（通常オブジェクト・仮置き）

    Tree,       // 木（仮置き）
    Box,        // 箱（仮置き）
    Rock,       // 岩（仮置き）
    Pillar,     // 柱（仮置き）
    OilDrum     // オイル缶（仮置き）
};

#endif // __GRID_OBJECT_TYPE_H__