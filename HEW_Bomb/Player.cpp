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
    , m_isKeyReleased(true)
    , m_bombPlacementPosition(0, 0, 0)
    , m_bombPlacementRotation(0.0f, 0.0f, 0.0f)
    , m_isPlaceKeyReleased(true)
    , m_isRetrieveKeyReleased(true)
    , m_isDetonateKeyReleased(true)
    , m_selectedBombIndex(0)
    , m_isBombNextKeyReleased(true)
    , m_isBombPreviousKeyReleased(true)
    , m_pCamera(nullptr)
    , m_pBomb(nullptr)
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
}


//==================================================
// 更新

void Player::Update(
    Map& map,
    BombInventory& inventory,
    const GridCollision& gridCollision)
{
    GridManager& gridManager = map.GetGridManager();

    //float nPos[3] = { PlayerGetPosition().GetX(), PlayerGetPosition().GetY(), PlayerGetPosition().GetZ() };

    //// カメラからキャラクターの向きを取得
    //DirectX::XMFLOAT3 camPos = m_pCamera->GetPos(); // カメラの位置を取得
    //DirectX::XMVECTOR vCamPos = DirectX::XMLoadFloat3(&camPos); // camPosを計算用のデータに変換
    //DirectX::XMVECTOR vPos = DirectX::XMLoadFloat3(nPos[0], ); // プレイヤーの位置を計算用のデータに変換
    //DirectX::XMVECTOR vec = DirectX::XMVectorSubtract(vPos, vCamPos);// カメラからプレイヤーに向かうベクトルを計算
    //vec = DirectX::XMVector3Normalize(vec); // vecを長さ１のベクトルに変換


    //==================================================
    // ① プレイヤーの移動

    GridPosition nextPos = m_playerPos;
    bool isKeyPressed = false;

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
    // ② 爆弾の設置予定位置

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
    // ③ 爆弾の設置（Spaceキー）

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
    // ④ 爆弾の回収（Rキー）

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
    // ⑤ 爆弾の起爆（Eキー）

    // 登録された起爆スポットをすべて確認する。
    // 爆弾が置かれているスポットだけ起爆する。

    bool isDetonateKeyPressed =
        (GetAsyncKeyState('E') & 0x8000) != 0;

    if (isDetonateKeyPressed)
    {
        if (m_isDetonateKeyReleased)
        {
            for (const auto& spot : map.GetDetonationSpots())
            {
                const GridPosition& position =
                    spot.GetPosition();

                if (!map.HasBombAt(position))
                {
                    continue;
                }

                map.DetonateBombAt(position);
            }

            m_isDetonateKeyReleased = false;
        }
    }
    else
    {
        m_isDetonateKeyReleased = true;
    }


    //==================================================
    // ⑥ 爆弾の種類切り替え（テスト用）

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



// 引数に GridManager& を受け取るようにする
bool Player::IsDeathPlayer(GridManager& gridManager)
{
    // m_pBomb が nullptr の場合は安全に抜ける（アクセス違反防止）
    if (m_pBomb == nullptr)
    {
        return false;
    }

    // 正しいゲーム全体の GridManager を使って影響範囲を取得
    std::vector<GridPosition> affectedPositions = m_pBomb->GetAffectedPositions(gridManager);

    bool isPlayerHit = false;
    for (const auto& pos : affectedPositions)
    {
        if (gridManager.HasPlayer(pos))
        {
            isPlayerHit = true;
            break;
        }
    }

    if (isPlayerHit)
    {
        m_nLife--;
    }

    return isPlayerHit; // 常に true ではなく、ヒットしたかどうかを返す
}
