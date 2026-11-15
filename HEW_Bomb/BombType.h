#pragma once
#ifndef __BOMB_TYPE_H__
#define __BOMB_TYPE_H__

// ”š’e‚Ìí—Ş
enum class BombType
{
    None = 0,

    Cross,              // \š
    Cylinder,           // ‰~’Œ
    WideCube,           // ‰¡’·•ûŒ`
    VShape,             // Vš
    Cube,               // —§•û‘Ì
    Circle,             // ‰~Œ^
    InvertedPyramid,    // ‹tOŠp

    Count               // ”š’e‚Ìí—Ş”‚ğæ“¾‚·‚é‚½‚ß‚Ì”Ô•º
};

// ”š’e‚Ìí—Ş”
inline int GetBombTypeCount()
{
    return static_cast<int>(BombType::Count) - 1;
}

// ƒCƒ“ƒfƒbƒNƒX‚©‚ç”š’e‚Ìí—Ş‚ğæ“¾
inline BombType GetBombTypeByIndex(int index)
{
    if (index < 0 || index >= GetBombTypeCount())
        return BombType::None;

    return static_cast<BombType>(index + 1);
}

#endif