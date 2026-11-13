#include "Player.h"
#include "GridManager.h"
#include "GridCollision.h"
#include <Windows.h>

Player::Player()
    : m_playerPos(0, 0, 0)
    , m_isKeyReleased(true)
{
}

Player::~Player() {}

void Player::Initialize(const GridPosition& startPos)
{
    m_playerPos = startPos;
}

// 毎フレームの移動処理（I, J, K, L キーで操作）
void Player::Update(GridManager& gridManager, const GridCollision& gridCollision)
{
    GridPosition nextPos = m_playerPos;
    bool isKeyPressed = false;

    if (GetAsyncKeyState('I') & 0x8000) { nextPos = m_playerPos + GridPosition(0, 0, 1); isKeyPressed = true; }
    else if (GetAsyncKeyState('K') & 0x8000) { nextPos = m_playerPos + GridPosition(0, 0, -1); isKeyPressed = true; }
    else if (GetAsyncKeyState('J') & 0x8000) { nextPos = m_playerPos + GridPosition(-1, 0, 0); isKeyPressed = true; }
    else if (GetAsyncKeyState('L') & 0x8000) { nextPos = m_playerPos + GridPosition(1, 0, 0); isKeyPressed = true; }

    if (isKeyPressed)
    {
        if (m_isKeyReleased)
        {
            if (nextPos != m_playerPos)
            {
                // 壁やマップの範囲外でないかチェックしてから移動
                if (gridCollision.CanMove(nextPos))
                {
                    if (gridManager.MovePlayer(m_playerPos, nextPos))
                    {
                        m_playerPos = nextPos;
                    }
                }
            }
            m_isKeyReleased = false;
        }
    }
    else
    {
        m_isKeyReleased = true;
    }
}