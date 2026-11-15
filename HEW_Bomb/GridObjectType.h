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

    NetworkServer,   
    Computer,
    Generator,

    Pillar,
    OilCan,

    Floor,

    Wall,       
};

#endif // __GRID_OBJECT_TYPE_H__