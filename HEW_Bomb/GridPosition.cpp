#include "GridPosition.h"

//==================================================
// コンストラクタ

// 各座標を0で初期化する
GridPosition::GridPosition()
    : m_x(0)
    , m_y(0)
    , m_z(0)
{
}

// デストラクタ
GridPosition::~GridPosition()
{
}

// 指定されたX・Y・Z座標で初期化する
GridPosition::GridPosition(int x, int y, int z)
    : m_x(x)
    , m_y(y)
    , m_z(z)
{
}

//==================================================
// 座標取得

// X座標を取得する
int GridPosition::GetX() const
{
    return m_x;
}

// Y座標を取得する
int GridPosition::GetY() const
{
    return m_y;
}

// Z座標を取得する
int GridPosition::GetZ() const
{
    return m_z;
}

//==================================================
// 座標設定

// X・Y・Z座標をまとめて設定する
void GridPosition::Set(int x, int y, int z)
{
    m_x = x;
    m_y = y;
    m_z = z;
}

//==================================================
// 座標演算

// 2つの座標を加算した新しい座標を返す
GridPosition GridPosition::operator+(const GridPosition& other) const
{
    // X・Y・Zそれぞれの座標を加算して新しい座標を作成する
    return GridPosition(
        m_x + other.m_x,
        m_y + other.m_y,
        m_z + other.m_z
    );
}

// 2つの座標を減算した新しい座標を返す
GridPosition GridPosition::operator-(const GridPosition& other) const
{
    // X・Y・Zそれぞれの座標を減算して新しい座標を作成する
    return GridPosition(
        m_x - other.m_x,
        m_y - other.m_y,
        m_z - other.m_z
    );
}

// 現在の座標に指定された座標を加算する
// 計算結果は現在の座標に反映される
GridPosition& GridPosition::operator+=(const GridPosition& other)
{
    // 各座標に指定された座標を加算する
    m_x += other.m_x;
    m_y += other.m_y;
    m_z += other.m_z;

    // 変更後の自分自身を返す
    return *this;
}

// 現在の座標から指定された座標を減算する
// 計算結果は現在の座標に反映される
GridPosition& GridPosition::operator-=(const GridPosition& other)
{
    // 各座標から指定された座標を減算する
    m_x -= other.m_x;
    m_y -= other.m_y;
    m_z -= other.m_z;

    // 変更後の自分自身を返す
    return *this;
}

//==================================================
// 座標比較

// X・Y・Zのすべての座標が一致しているか確認する
bool GridPosition::operator==(const GridPosition& other) const
{
    // 3つの座標がすべて一致している場合にtrueを返す
    return m_x == other.m_x
        && m_y == other.m_y
        && m_z == other.m_z;
}

// X・Y・Zのいずれかの座標が異なっているか確認する
bool GridPosition::operator!=(const GridPosition& other) const
{
    // ==の結果を反転させることで、不一致を判定する
    return !(*this == other);
}