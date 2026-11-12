#include "GridCell.h"

// コンストラクタ
GridCell::GridCell()
    : m_position()
    , m_isWall(false)
    , m_objectType(GridObjectType::None)
    , m_hasBomb(false)
    , m_hasPlayer(false)
    , m_hasFloor(false)
{
}

// デストラクタ
GridCell::~GridCell()
{
}


// 指定されたグリッド座標を持つセルを作成する
GridCell::GridCell(const GridPosition& position)
    : m_position(position)
    , m_isWall(false)
    , m_objectType(GridObjectType::None)
    , m_hasBomb(false)
    , m_hasPlayer(false)
    , m_hasFloor(false)
{
}


//=========================================================
// 情報取得

// マスのグリッド座標を取得する
const GridPosition& GridCell::GetPosition() const
{
    return m_position;
}

// マスに壁が設置されているか確認する
bool GridCell::IsWall() const
{
    return m_isWall;
}

// マスにオブジェクトが設置されているか確認する
bool GridCell::HasObject() const
{
    // オブジェクトの種類がNone以外なら、オブジェクトがあると判定する
    return m_objectType != GridObjectType::None;
}

// マスに爆弾が設置されているか確認する
bool GridCell::HasBomb() const
{
    return m_hasBomb;
}

// マスにプレイヤーがいるか確認する
bool GridCell::HasPlayer() const
{
    return m_hasPlayer;
}


//==================================================
// 状態設定

// マスの壁の有無を設定する
void GridCell::SetWall(bool wall)
{
    m_isWall = wall;
}

// マスのオブジェクトの有無を設定する
// trueの場合はDefault、falseの場合はNoneに設定する
void GridCell::SetObject(bool object)
{
    if (object)
    {
        m_objectType = GridObjectType::Default;
    }
    else
    {
        m_objectType = GridObjectType::None;
    }
}

// マスの爆弾の有無を設定する
void GridCell::SetBomb(bool bomb)
{
    m_hasBomb = bomb;
}

// マスのプレイヤーの有無を設定する
void GridCell::SetPlayer(bool player)
{
    m_hasPlayer = player;
}


//==================================================
// 移動判定

// マスが移動可能か確認する
// 壁・オブジェクト・爆弾がある場合は移動不可とする
bool GridCell::IsWalkable() const
{
    // 壁がある場合は移動不可
    if (m_isWall)
    {
        return false;
    }

    // オブジェクトがある場合は移動不可
    if (HasObject())
    {
        return false;
    }

    // 爆弾がある場合は移動不可
    if (m_hasBomb)
    {
        return false;
    }

    // どの障害物もない場合は移動可能
    return true;
}


//==================================================
// オブジェクト情報

// 設置されているオブジェクトの種類を取得する
// オブジェクトがない場合はGridObjectType::Noneを返す
GridObjectType GridCell::GetObjectType() const
{
    return m_objectType;
}

// マスに設置するオブジェクトの種類を設定する
void GridCell::SetObjectType(GridObjectType type)
{
    m_objectType = type;
}

// 床があるか
bool GridCell::HasFloor() const
{
    return m_hasFloor;
}

// 床の設定
void GridCell::SetFloor(bool floor)
{
    m_hasFloor = floor;
}