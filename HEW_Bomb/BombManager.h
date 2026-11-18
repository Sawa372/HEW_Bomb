#pragma once
#ifndef __BOMB_MANAGER_H__
#define __BOMB_MANAGER_H__

#include <vector>
#include <DirectXMath.h>
#include <memory>

#include "Bomb.h"


class GridManager;
class BombInventory;

class BombManager
{
public:
    BombManager();
    ~BombManager();

    //// ‘I‘ğ’†‚Ì”š’e‚ğİ’u‚µ‚Ä“o˜^‚·‚é
    //bool AddBomb(
    //    const GridPosition& position,
    //    GridManager& grid,
    //    BombInventory& inventory
    //);

    // ‰ñ“]‚ğw’è‚µ‚Ä”š’e‚ğİ’u‚·‚é
    bool AddBomb(
        const GridPosition& position,
        const DirectX::XMFLOAT3& rotation,
        GridManager& grid,
        BombInventory& inventory
    );

    // w’èˆÊ’u‚Ì”š’e‚ğæ“¾‚·‚é
    Bomb* GetBombAt(const GridPosition& position);
    const Bomb* GetBombAt(const GridPosition& position) const;

    // w’èˆÊ’u‚É”š’e‚ª‚ ‚é‚©
    bool HasBombAt(const GridPosition& position) const;

    // w’èˆÊ’u‚Ì”š’e‚ğ‰ñû‚µ‚Äíœ‚·‚é
    bool RemoveBombAt(
        const GridPosition& position,
        GridManager& grid,
        BombInventory& inventory
    );

    // ”š”­Ï‚İ‚Ì”š’e‚ğíœ‚·‚é
    void RemoveExplodedBombs(GridManager& grid);

    // ‚·‚×‚Ä‚Ì”š’e‚ğ‰ñû‚µ‚Äíœ‚·‚é
    void Clear(GridManager& grid);

    // “o˜^‚³‚ê‚Ä‚¢‚é”š’e‚Ì”
    int GetBombCount() const;


    // Undo—p‚Ìó‘Ô‚ğ•Û‘¶
    bool SaveUndoState();

    // •Û‘¶‚µ‚½ó‘Ô‚É–ß‚·
    bool RestoreUndoState();

    // •Û‘¶‚µ‚½ó‘Ô‚ğ”jŠü
    void DiscardUndoState();


private:
    std::vector<std::unique_ptr<Bomb>> m_bombs;

    // Undo—p‚Ì”š’e•Û‘¶ƒf[ƒ^
    std::vector<Bomb> m_savedBombs;

    // •Û‘¶ƒf[ƒ^‚ª‘¶İ‚·‚é‚©
    bool m_hasSavedState;
};

#endif
