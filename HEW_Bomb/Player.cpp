#include "Player.h"

#include "Map.h"
#include "GridManager.h"
#include "GridCollision.h"
#include "BombInventory.h"
#include "BombType.h"
#include "Explosion.h"

#include <Windows.h>


//==================================================
// コンストラクタ

Player::Player()
    : m_playerPos(0, 0, 0)
    , m_nLife(3)
    , m_startPos(0, 0, 0)
    , m_isKeyReleased(true)
    , m_bombPlacementPosition(0, 0, 0)
    , m_bombPlacementRotation(0.0f, 0.0f, 0.0f)
    , m_isPlaceKeyReleased(true)
    , m_isRetrieveKeyReleased(true)
    , m_isDetonateKeyReleased(true)
    , m_selectedBombIndex(0)
    , m_isBombNextKeyReleased(true)
    , m_isBombPreviousKeyReleased(true)
    , m_isRotateXKeyReleased(true)
    , m_isRotateYKeyReleased(true)
    , m_pCamera(nullptr)
    , m_pBomb(nullptr)
	, m_bombRotation()
{
}

//==================================================
// デストラクタ

Player::~Player()
{
}


//==================================================
// 初期化

void Player::Initialize(const GridPosition& startPos)
{
    m_playerPos = startPos;
    m_bombPlacementPosition = startPos;

    // 爆弾の回転を初期化
    m_bombRotation.Reset();

    // 初期回転角度を取得
    m_bombPlacementRotation =
        m_bombRotation.GetEulerAngles();

    m_playerPos = startPos;
    m_startPos = startPos;
    m_bombPlacementPosition = startPos;

}

//==================================================
// 更新

void Player::Update(
    Map& map,
    BombInventory& inventory,
    const GridCollision& gridCollision)
{
    GridManager& gridManager = map.GetGridManager();

    

    //==================================================
    //  プレイヤーの移動

    GridPosition nextPos = m_playerPos;
    bool isKeyPressed = false;

    if (m_pCamera->GetPos().z <= 0.0f && abs(m_pCamera->GetPos().x) <= 2.5f) // Z+が正面
    {
        if (GetAsyncKeyState('I') & 0x8000)
        {
            nextPos = m_playerPos + GridPosition(0, 0, 1);
            isKeyPressed = true;
        }
        else if (GetAsyncKeyState('K') & 0x8000)
        {
            nextPos = m_playerPos + GridPosition(0, 0, -1);
            isKeyPressed = true;
        }
        else if (GetAsyncKeyState('J') & 0x8000)
        {
            nextPos = m_playerPos + GridPosition(-1, 0, 0);
            isKeyPressed = true;
        }
        else if (GetAsyncKeyState('L') & 0x8000)
        {
            nextPos = m_playerPos + GridPosition(1, 0, 0);
            isKeyPressed = true;
        }
        else if (GetAsyncKeyState('7') & 0x8000)
        {
            nextPos = m_playerPos + GridPosition(0, 1, 0);
            isKeyPressed = true;
        }
        else if (GetAsyncKeyState('8') & 0x8000)
        {
            nextPos = m_playerPos + GridPosition(0, -1, 0);
            isKeyPressed = true;
        }
    }
    else if (m_pCamera->GetPos().z >= 0.0f && abs(m_pCamera->GetPos().x) <= 2.5f) // Z-が正面
    {
        if (GetAsyncKeyState('I') & 0x8000)
        {
            nextPos = m_playerPos + GridPosition(0, 0, -1);
            isKeyPressed = true;
        }
        else if (GetAsyncKeyState('K') & 0x8000)
        {
            nextPos = m_playerPos + GridPosition(0, 0, 1);
            isKeyPressed = true;
        }
        else if (GetAsyncKeyState('J') & 0x8000)
        {
            nextPos = m_playerPos + GridPosition(1, 0, 0);
            isKeyPressed = true;
        }
        else if (GetAsyncKeyState('L') & 0x8000)
        {
            nextPos = m_playerPos + GridPosition(-1, 0, 0);
            isKeyPressed = true;
        }
        else if (GetAsyncKeyState('7') & 0x8000)
        {
            nextPos = m_playerPos + GridPosition(0, 1, 0);
            isKeyPressed = true;
        }
        else if (GetAsyncKeyState('8') & 0x8000)
        {
            nextPos = m_playerPos + GridPosition(0, -1, 0);
            isKeyPressed = true;
        }
    }
    else if (abs(m_pCamera->GetPos().z) <= 2.5f && m_pCamera->GetPos().x <= 0.0f) // X+が正面
    {
        if (GetAsyncKeyState('I') & 0x8000)
        {
            nextPos = m_playerPos + GridPosition(1, 0, 0);
            isKeyPressed = true;
        }
        else if (GetAsyncKeyState('K') & 0x8000)
        {
            nextPos = m_playerPos + GridPosition(-1, 0, 0);
            isKeyPressed = true;
        }
        else if (GetAsyncKeyState('J') & 0x8000)
        {
            nextPos = m_playerPos + GridPosition(0, 0, 1);
            isKeyPressed = true;
        }
        else if (GetAsyncKeyState('L') & 0x8000)
        {
            nextPos = m_playerPos + GridPosition(0, 0, -1);
            isKeyPressed = true;
        }
        else if (GetAsyncKeyState('7') & 0x8000)
        {
            nextPos = m_playerPos + GridPosition(0, 1, 0);
            isKeyPressed = true;
        }
        else if (GetAsyncKeyState('8') & 0x8000)
        {
            nextPos = m_playerPos + GridPosition(0, -1, 0);
            isKeyPressed = true;
        }
    }
    else if (abs(m_pCamera->GetPos().z) <= 2.5f && m_pCamera->GetPos().x >= 0.0f) // X-が正面
    {
        if (GetAsyncKeyState('I') & 0x8000)
        {
            nextPos = m_playerPos + GridPosition(-1, 0, 0);
            isKeyPressed = true;
        }
        else if (GetAsyncKeyState('K') & 0x8000)
        {
            nextPos = m_playerPos + GridPosition(1, 0, 0);
            isKeyPressed = true;
        }
        else if (GetAsyncKeyState('J') & 0x8000)
        {
            nextPos = m_playerPos + GridPosition(0, 0, -1);
            isKeyPressed = true;
        }
        else if (GetAsyncKeyState('L') & 0x8000)
        {
            nextPos = m_playerPos + GridPosition(0, 0, 1);
            isKeyPressed = true;
        }
        else if (GetAsyncKeyState('7') & 0x8000)
        {
            nextPos = m_playerPos + GridPosition(0, 1, 0);
            isKeyPressed = true;
        }
        else if (GetAsyncKeyState('8') & 0x8000)
        {
            nextPos = m_playerPos + GridPosition(0, -1, 0);
            isKeyPressed = true;
        }
    }
    

    if (isKeyPressed)
    {
        if (m_isKeyReleased)
        {
            if (nextPos != m_playerPos)
            {
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


    //==================================================
    //  爆弾の設置予定位置

    // 【現在のテスト処理】
    // 現在はプレイヤーと同じ座標を設置予定位置にしている。
    //
    // 【正式実装時】
    // プレイヤーの向いている方向を取得し、
    // プレイヤーの前方1マスを設置予定位置にする。
    //
    // プレイヤーの向きに合わせて回転も設定する。

    m_bombPlacementPosition = m_playerPos;


    //==================================================
    //  爆弾の設置（Spaceキー）

    bool isPlaceKeyPressed =
        (GetAsyncKeyState(VK_SPACE) & 0x8000) != 0;

    if (isPlaceKeyPressed)
    {
        if (m_isPlaceKeyReleased)
        {
            map.AddBomb(
                m_bombPlacementPosition,
                m_bombPlacementRotation,
                inventory
            );

            m_isPlaceKeyReleased = false;
        }
    }
    else
    {
        m_isPlaceKeyReleased = true;
    }


    //==================================================
    //  爆弾の回収（Rキー）

    bool isRetrieveKeyPressed =
        (GetAsyncKeyState('R') & 0x8000) != 0;

    if (isRetrieveKeyPressed)
    {
        if (m_isRetrieveKeyReleased)
        {
            map.RemoveBombAt(
                m_bombPlacementPosition,
                inventory
            );

            m_isRetrieveKeyReleased = false;
        }
    }
    else
    {
        m_isRetrieveKeyReleased = true;
    }


    //==================================================
    //  爆弾の起爆（Eキー）
    bool isDetonateKeyPressed =
        (GetAsyncKeyState('E') & 0x8000) != 0;

    if (isDetonateKeyPressed)
    {
        if (m_isDetonateKeyReleased)
        {
            // 起爆直前のプレイヤー位置を保存
            GridPosition playerPositionBefore =
                m_playerPos;

            GridPosition placementPositionBefore =
                m_bombPlacementPosition;

            // 起爆スポットに爆弾があるか確認
            bool hasBomb = false;

            for (const auto& spot : map.GetDetonationSpots())
            {
                if (map.HasBombAt(spot.GetPosition()))
                {
                    hasBomb = true;
                    break;
                }
            }

            if (hasBomb)
            {
                //==================================================
                // 起爆直前の状態を保存

                map.SetPlayerPosition(m_playerPos);

                if (map.SaveUndoState())
                {
                    std::vector<GridPosition> dangerousPositions;

                    //==================================================
                    // 爆発・連鎖爆発

                    for (const auto& spot : map.GetDetonationSpots())
                    {
                        const GridPosition& position =
                            spot.GetPosition();

                        if (!map.HasBombAt(position))
                        {
                            continue;
                        }

                        std::vector<GridPosition> bombAffectedPositions;

                        if (map.DetonateBombAt(
                            position,
                            &bombAffectedPositions))
                        {
                            dangerousPositions.insert(
                                dangerousPositions.end(),
                                bombAffectedPositions.begin(),
                                bombAffectedPositions.end()
                            );
                        }
                    }

                    //==================================================
                    // 死亡判定

                    if (map.IsPlayerKilledByFloor())
                    {
                        // 床による即死：残機を0にする
                        m_nLife = 0;

                        // Undoで爆発前の状態に戻さない
                        map.DiscardUndoState();
                    }
                    else if (CheckDeath(dangerousPositions))
                    {
                        // 残機を1つ減らす
                        m_nLife--;

                        // 起爆直前のMapに戻す
                        if (map.RestoreUndoState())
                        {
                            // プレイヤー自身の座標も戻す
                            m_playerPos = playerPositionBefore;

                            m_bombPlacementPosition =
                                placementPositionBefore;
                        }
                    }
                    else
                    {
                        // 生存した場合は保存データを破棄
                        map.DiscardUndoState();
                    }

                    map.ResetPlayerKilledByFloor();
                }
            }

            m_isDetonateKeyReleased = false;
        }
    }
    else
    {
        m_isDetonateKeyReleased = true;
    }

    //==================================================
    //  爆弾の種類切り替え（テスト用）

    // 1キー：次の爆弾
    bool isNextBombPressed =
        (GetAsyncKeyState('1') & 0x8000) != 0;

    if (isNextBombPressed)
    {
        if (m_isBombNextKeyReleased)
        {
            m_selectedBombIndex++;

            if (m_selectedBombIndex >= GetBombTypeCount())
            {
                m_selectedBombIndex = 0;
            }

            inventory.SelectBomb(
                GetBombTypeByIndex(m_selectedBombIndex)
            );

            m_isBombNextKeyReleased = false;
        }
    }
    else
    {
        m_isBombNextKeyReleased = true;
    }


    // 2キー：前の爆弾
    bool isPreviousBombPressed =
        (GetAsyncKeyState('2') & 0x8000) != 0;

    if (isPreviousBombPressed)
    {
        if (m_isBombPreviousKeyReleased)
        {
            m_selectedBombIndex--;

            if (m_selectedBombIndex < 0)
            {
                m_selectedBombIndex = GetBombTypeCount() - 1;
            }

            inventory.SelectBomb(
                GetBombTypeByIndex(m_selectedBombIndex)
            );

            m_isBombPreviousKeyReleased = false;
        }
    }
    else
    {
        m_isBombPreviousKeyReleased = true;
    }

    //==================================================
    // 爆弾の回転

    // 3キー：ワールドX軸を90度回転
    bool isRotateXKeyPressed =
        (GetAsyncKeyState('3') & 0x8000) != 0;

    if (isRotateXKeyPressed)
    {
        if (m_isRotateXKeyReleased)
        {
            m_bombRotation.RotateWorldX90();

            m_bombPlacementRotation =
                m_bombRotation.GetEulerAngles();

            m_isRotateXKeyReleased = false;
        }
    }
    else
    {
        m_isRotateXKeyReleased = true;
    }

    // 4キー：ワールドY軸を90度回転
    bool isRotateYKeyPressed =
        (GetAsyncKeyState('4') & 0x8000) != 0;

    if (isRotateYKeyPressed)
    {
        if (m_isRotateYKeyReleased)
        {
            m_bombRotation.RotateWorldY90();

            m_bombPlacementRotation =
                m_bombRotation.GetEulerAngles();

            m_isRotateYKeyReleased = false;
        }
    }
    else
    {
        m_isRotateYKeyReleased = true;
    }
}


//==================================================
// プレイヤーの現在位置を取得

GridPosition Player::PlayerGetPosition() const
{
    return m_playerPos;
}


//==================================================
// 爆弾の設置予定位置を取得

const GridPosition& Player::GetBombPlacementPosition() const
{
    return m_bombPlacementPosition;
}


//==================================================
// 爆弾の設置回転を取得

const DirectX::XMFLOAT3& Player::GetBombPlacementRotation() const
{
    return m_bombPlacementRotation;
}


//==================================================
// 爆弾の設置回転を設定

void Player::SetBombPlacementRotation(
    const DirectX::XMFLOAT3& rotation)
{
    m_bombPlacementRotation = rotation;
}

// カメラ情報を取得
void Player::SetCamera(Camera* pCamera)
{
    m_pCamera = pCamera;
}



//==================================================
// プレイヤーの死亡判定

bool Player::CheckDeath(
    const std::vector<GridPosition>& dangerousPositions) const
{
    // 危険範囲にプレイヤーがいるか確認
    for (const GridPosition& position : dangerousPositions)
    {
        if (position == m_playerPos)
        {
            return true;
        }
    }

    return false;
}

//==================================================
// 残機を取得

int Player::GetLife() const
{
    return m_nLife;
}