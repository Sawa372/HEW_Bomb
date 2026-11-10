#pragma once
#ifndef __GRID_OBJECT_TYPE_H__
#define __GRID_OBJECT_TYPE_H__

// グリッドに配置するオブジェクトの種類
enum class GridObjectType
{
    None,       // オブジェクトなし
    Default,    // 種類未指定(通常オブジェクト)

    Tree,       // 木
    Box,        // 箱
    Rock,       // 岩
    Pillar,     // 柱
    OilDrum     // オイル缶
};

#endif // __GRID_OBJECT_TYPE_H__