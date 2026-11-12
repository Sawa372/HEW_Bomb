#include <cstdlib>
#include <cmath>
#include <climits>

#include "GridManager.h"

// コンストラクタ
// 各メンバ変数を初期値に設定する
GridManager::GridManager()
    : m_minPosition(0, 0, 0)
    , m_width(0)
    , m_height(0)
    , m_depth(0)
    , m_cellSize(1.0f, 1.0f, 1.0f)
    , m_gridOrigin(0.0f, 0.0f, 0.0f)
{
}

// デストラクタ
// グリッドのセル情報を解放する
GridManager::~GridManager()
{
    Clear();
}


//==================================================
// グリッドの範囲取得

// グリッドの最小座標と最大座標を取得する
// グリッドが初期化されていない場合はfalseを返す
bool GridManager::GetBounds(
    GridPosition& minPosition,
    GridPosition& maxPosition
) const
{
    // グリッドが初期化されていない場合は取得できない
    if (m_cells.empty())
    {
        return false;
    }

    // グリッドの最小座標を取得
    minPosition = m_minPosition;

    // 最小座標と各方向のマス数から最大座標を計算
    maxPosition.Set(
        m_minPosition.GetX() + m_width - 1,
        m_minPosition.GetY() + m_height - 1,
        m_minPosition.GetZ() + m_depth - 1
    );

    return true;
}


//==================================================
// グリッドの初期化

// 指定した幅・高さ・奥行きでグリッドを作成する
// 座標の開始位置は(0, 0, 0)となる
bool GridManager::Initialize(int width, int height, int depth)
{
    // 各方向のサイズが1未満の場合は作成できない
    if (width <= 0 || height <= 0 || depth <= 0)
    {
        return false;
    }

    // 最小座標を(0, 0, 0)として、範囲指定による初期化を行う
    return Initialize(
        GridPosition(0, 0, 0),
        GridPosition(width - 1, height - 1, depth - 1)
    );
}

// 指定した最小座標から最大座標までの範囲でグリッドを作成する
// マイナス座標を含む範囲にも対応する
bool GridManager::Initialize(
    const GridPosition& minPosition,
    const GridPosition& maxPosition
)
{
    // 範囲の確認

    // 最小座標が最大座標を超えている場合は作成できない
    if (minPosition.GetX() > maxPosition.GetX() ||
        minPosition.GetY() > maxPosition.GetY() ||
        minPosition.GetZ() > maxPosition.GetZ())
    {
        return false;
    }

    // グリッドのサイズ計算

    // 最大座標と最小座標から各方向のマス数を計算する
    // long longを使用して、計算途中のintのオーバーフローを防ぐ
    long long width =
        static_cast<long long>(maxPosition.GetX()) -
        minPosition.GetX() + 1;

    long long height =
        static_cast<long long>(maxPosition.GetY()) -
        minPosition.GetY() + 1;

    long long depth =
        static_cast<long long>(maxPosition.GetZ()) -
        minPosition.GetZ() + 1;

    // 各方向のマス数がintで扱える範囲か確認する
    if (width > INT_MAX || height > INT_MAX || depth > INT_MAX)
    {
        return false;
    }

    // グリッド全体のマス数がintで扱える範囲か確認する
    // 配列のインデックス計算で使用するため、上限を超える場合は作成しない
    if (width * height > INT_MAX ||
        width * height * depth > INT_MAX)
    {
        return false;
    }

    // グリッドの作成準備

    // 古いグリッドのセル情報を解放
    Clear();

    // 新しいグリッドの最小座標を保存
    m_minPosition = minPosition;

    // 各方向のマス数を保存
    m_width = static_cast<int>(width);
    m_height = static_cast<int>(height);
    m_depth = static_cast<int>(depth);

    // グリッド全体に必要なセル数を事前に確保する
    m_cells.reserve(
        static_cast<size_t>(m_width) *
        m_height *
        m_depth
    );

    // グリッドの作成

    // Y・Z・Xの順番でマスを作成する
    for (int y = 0; y < m_height; ++y)
    {
        for (int z = 0; z < m_depth; ++z)
        {
            for (int x = 0; x < m_width; ++x)
            {
                // 最小座標を基準に、実際のグリッド座標を設定する
                m_cells.emplace_back(
                    GridPosition(
                        minPosition.GetX() + x,
                        minPosition.GetY() + y,
                        minPosition.GetZ() + z
                    )
                );
            }
        }
    }

    return true;
}

// グリッドの解放

// グリッド内のセルと範囲情報をクリアする
// セルサイズとワールド原点の設定は保持する
void GridManager::Clear()
{
    // すべてのセル情報を削除する
    m_cells.clear();

    // 最小座標を初期値に戻す
    m_minPosition.Set(0, 0, 0);

    // グリッドのサイズを0に戻す
    m_width = 0;
    m_height = 0;
    m_depth = 0;
}


//==================================================
// グリッドのサイズ取得

// グリッドのX方向のマス数を取得する
int GridManager::GetWidth() const
{
    return m_width;
}

// グリッドのY方向のマス数を取得する
int GridManager::GetHeight() const
{
    return m_height;
}

// グリッドのZ方向のマス数を取得する
int GridManager::GetDepth() const
{
    return m_depth;
}


//==================================================
// グリッドの範囲判定

// 指定した座標がグリッドの範囲内にあるか確認する
bool GridManager::IsInside(const GridPosition& position) const
{
    // 座標をlong longに変換して、範囲計算時のオーバーフローを防ぐ
    long long x = position.GetX();
    long long y = position.GetY();
    long long z = position.GetZ();

    long long minX = m_minPosition.GetX();
    long long minY = m_minPosition.GetY();
    long long minZ = m_minPosition.GetZ();

    // 各座標が最小座標以上、最小座標+マス数未満なら範囲内
    return
        x >= minX && x < minX + m_width &&
        y >= minY && y < minY + m_height &&
        z >= minZ && z < minZ + m_depth;
}


//==================================================
// セルの取得

// 指定した座標のマスを取得する
// 範囲外の場合はnullptrを返す
GridCell* GridManager::GetCell(const GridPosition& position)
{
    // グリッドの範囲外なら取得できない
    if (!IsInside(position))
    {
        return nullptr;
    }

    // グリッド座標に対応する配列のセルを返す
    return &m_cells[GetIndex(position)];
}

// 指定した座標のマスを取得する（読み取り専用）
// 範囲外の場合はnullptrを返す
const GridCell* GridManager::GetCell(const GridPosition& position) const
{
    // グリッドの範囲外なら取得できない
    if (!IsInside(position))
    {
        return nullptr;
    }

    // グリッド座標に対応する配列のセルを返す
    return &m_cells[GetIndex(position)];
}

// グリッド内のすべてのマスを取得する（読み取り専用）
const std::vector<GridCell>& GridManager::GetCells() const
{
    return m_cells;
}


//==================================================
// マスの状態確認

// 指定したマスに壁があるか確認する
// 範囲外の場合は壁があるものとして扱う
bool GridManager::IsWall(const GridPosition& position) const
{
    const GridCell* cell = GetCell(position);

    // 範囲外への移動や配置を防ぐため、範囲外は壁として扱う
    if (cell == nullptr)
    {
        return true;
    }

    return cell->IsWall();
}

// 指定したマスにオブジェクトがあるか確認する
bool GridManager::HasObject(const GridPosition& position) const
{
    const GridCell* cell = GetCell(position);

    // 範囲外の場合はオブジェクトがないものとして扱う
    if (cell == nullptr)
    {
        return false;
    }

    return cell->HasObject();
}

// 指定したマスに爆弾があるか確認する
bool GridManager::HasBomb(const GridPosition& position) const
{
    const GridCell* cell = GetCell(position);

    // 範囲外の場合は爆弾がないものとして扱う
    if (cell == nullptr)
    {
        return false;
    }

    return cell->HasBomb();
}

// 指定したマスにプレイヤーがいるか確認する
bool GridManager::HasPlayer(const GridPosition& position) const
{
    const GridCell* cell = GetCell(position);

    // 範囲外の場合はプレイヤーがいないものとして扱う
    if (cell == nullptr)
    {
        return false;
    }

    return cell->HasPlayer();
}


//==================================================
// 移動判定

// 指定したマスが移動可能か確認する
bool GridManager::CanMove(const GridPosition& position) const
{
    const GridCell* cell = GetCell(position);

    // 範囲外のマスには移動できない
    if (cell == nullptr)
    {
        return false;
    }

    // 通行できる状態で、プレイヤーもいない場合のみ移動可能
    return cell->IsWalkable() && !cell->HasPlayer();
}


//==================================================
// 壁の配置・削除

// 指定したマスに壁を設置する
bool GridManager::SetWall(const GridPosition& position)
{
    // 壁を配置できる状態か確認する
    if (!CanPlaceWall(position))
    {
        return false;
    }

    GridCell* cell = GetCell(position);

    // セルの壁情報を有効にする
    cell->SetWall(true);

    return true;
}

// 指定したマスの壁を削除する
bool GridManager::RemoveWall(const GridPosition& position)
{
    GridCell* cell = GetCell(position);

    // セルが存在しない、または壁がない場合は削除できない
    if (cell == nullptr || !cell->IsWall())
    {
        return false;
    }

    // セルの壁情報を無効にする
    cell->SetWall(false);

    return true;
}


//==================================================
// オブジェクトの配置・削除

// 指定したマスにDefaultオブジェクトを設置する
bool GridManager::SetObject(const GridPosition& position)
{
    // 種類を指定しない場合はDefaultにする
    return SetObject(position, GridObjectType::Default);
}

// 指定した種類のオブジェクトをマスに設置する
bool GridManager::SetObject(
    const GridPosition& position,
    GridObjectType type
)
{
    // Noneはオブジェクトとして配置できない
    if (type == GridObjectType::None)
    {
        return false;
    }

    // 指定したマスにオブジェクトを配置できるか確認する
    if (!CanPlaceObject(position))
    {
        return false;
    }

    GridCell* cell = GetCell(position);

    // セルが取得できなかった場合は配置しない
    if (cell == nullptr)
    {
        return false;
    }

    // 指定されたオブジェクトの種類をセルに設定する
    cell->SetObjectType(type);

    return true;
}

// 指定したマスのオブジェクトを削除する
bool GridManager::RemoveObject(const GridPosition& position)
{
    GridCell* cell = GetCell(position);

    // セルが存在しない、またはオブジェクトがない場合は削除できない
    if (cell == nullptr || !cell->HasObject())
    {
        return false;
    }

    // オブジェクトを削除するため、種類をNoneに戻す
    cell->SetObject(false);

    return true;
}

// オブジェクトが配置されているすべての座標を取得する
std::vector<GridPosition> GridManager::GetObjectPositions() const
{
    std::vector<GridPosition> positions;

    // グリッド内のすべてのセルを確認する
    for (const GridCell& cell : m_cells)
    {
        // オブジェクトがあるセルの座標をリストに追加する
        if (cell.HasObject())
        {
            positions.push_back(cell.GetPosition());
        }
    }

    return positions;
}

// 壁が配置されているすべての座標を取得する
std::vector<GridPosition> GridManager::GetWallPositions() const
{
    std::vector<GridPosition> positions;

    // グリッド内のすべてのセルを確認する
    for (const GridCell& cell : m_cells)
    {
        // 壁があるセルの座標をリストに追加する
        if (cell.IsWall())
        {
            positions.push_back(cell.GetPosition());
        }
    }

    return positions;
}


//==================================================
// 爆弾の配置・削除

// 指定したマスに爆弾を設置する
bool GridManager::SetBomb(const GridPosition& position)
{
    // 爆弾を設置できる状態か確認する
    if (!CanPlaceBomb(position))
    {
        return false;
    }

    GridCell* cell = GetCell(position);

    // セルの爆弾情報を有効にする
    cell->SetBomb(true);

    return true;
}

// 指定したマスの爆弾を削除する
bool GridManager::RemoveBomb(const GridPosition& position)
{
    GridCell* cell = GetCell(position);

    // セルが存在しない、または爆弾がない場合は削除できない
    if (cell == nullptr || !cell->HasBomb())
    {
        return false;
    }

    // セルの爆弾情報を無効にする
    cell->SetBomb(false);

    return true;
}


//==================================================
// プレイヤーの配置・削除

// 指定したマスにプレイヤーを配置する
bool GridManager::SetPlayer(const GridPosition& position)
{
    // プレイヤーを配置できる状態か確認する
    if (!CanPlacePlayer(position))
    {
        return false;
    }

    GridCell* cell = GetCell(position);

    // セルのプレイヤー情報を有効にする
    cell->SetPlayer(true);

    return true;
}

// 指定したマスのプレイヤーを削除する
bool GridManager::RemovePlayer(const GridPosition& position)
{
    GridCell* cell = GetCell(position);

    // セルが存在しない、またはプレイヤーがいない場合は削除できない
    if (cell == nullptr || !cell->HasPlayer())
    {
        return false;
    }

    // セルのプレイヤー情報を無効にする
    cell->SetPlayer(false);

    return true;
}


//==================================================
// 内部処理

// グリッド座標を配列のインデックスに変換する
int GridManager::GetIndex(const GridPosition& position) const
{
    // 最小座標を基準とした配列内の相対座標を求める
    int x = position.GetX() - m_minPosition.GetX();
    int y = position.GetY() - m_minPosition.GetY();
    int z = position.GetZ() - m_minPosition.GetZ();

    // X方向を最も細かい単位として、1次元配列のインデックスを計算する
    // X方向に1マス進むと+1、Z方向はX方向のマス数分進む
    // Y方向はX方向とZ方向のマス数を掛けた分だけ進む
    return y * m_depth * m_width
        + z * m_width
        + x;
}


//==================================================
// プレイヤーの移動

// プレイヤーを現在のマスから次のマスへ移動させる
bool GridManager::MovePlayer(
    const GridPosition& currentPosition,
    const GridPosition& nextPosition)
{

    // 移動距離の確認

    // 現在地から移動先までのX・Y・Z方向の距離を計算する
    int dx = std::abs(nextPosition.GetX() - currentPosition.GetX());
    int dy = std::abs(nextPosition.GetY() - currentPosition.GetY());
    int dz = std::abs(nextPosition.GetZ() - currentPosition.GetZ());

    // 隣接する1マス以外への移動は禁止
    // 斜め移動や複数マス移動もここで防ぐ
    if (dx + dy + dz != 1)
    {
        return false;
    }


    // 現在地の確認

    // 現在のプレイヤー位置に対応するセルを取得する
    GridCell* currentCell = GetCell(currentPosition);

    // 現在地が範囲外、またはプレイヤーがいない場合は移動できない
    if (currentCell == nullptr || !currentCell->HasPlayer())
    {
        return false;
    }


    // 移動先の確認

    // 移動先が通行可能か確認する
    if (!CanMove(nextPosition))
    {
        return false;
    }

    // 移動先のセルを取得する
    GridCell* nextCell = GetCell(nextPosition);

    // 移動先のセルが取得できなかった場合は移動しない
    if (nextCell == nullptr)
    {
        return false;
    }


    // プレイヤーの移動

    // 現在地からプレイヤー情報を取り除く
    currentCell->SetPlayer(false);

    // 移動先にプレイヤー情報を設定する
    nextCell->SetPlayer(true);

    return true;
}


//==================================================
// 配置可能判定

// 指定したマスに爆弾を設置できるか確認する
bool GridManager::CanPlaceBomb(const GridPosition& position) const
{
    const GridCell* cell = GetCell(position);

    // 範囲外のマスには設置できない
    if (cell == nullptr)
    {
        return false;
    }

    // 壁・オブジェクト・爆弾がある場所には設置できない
    // プレイヤーがいる場所には設置できる
    return !cell->IsWall()
        && !cell->HasObject()
        && !cell->HasBomb();
}

// 指定したマスに壁を設置できるか確認する
bool GridManager::CanPlaceWall(const GridPosition& position) const
{
    const GridCell* cell = GetCell(position);

    // 範囲外のマスには設置できない
    if (cell == nullptr)
    {
        return false;
    }

    // すでに壁・オブジェクト・爆弾・プレイヤーがある場所には設置できない
    return !cell->IsWall()
        && !cell->HasObject()
        && !cell->HasBomb()
        && !cell->HasPlayer();
}

// 指定したマスにオブジェクトを設置できるか確認する
bool GridManager::CanPlaceObject(const GridPosition& position) const
{
    const GridCell* cell = GetCell(position);

    // 範囲外のマスには設置できない
    if (cell == nullptr)
    {
        return false;
    }

    // すでに壁・オブジェクト・爆弾・プレイヤーがある場所には設置できない
    return !cell->IsWall()
        && !cell->HasObject()
        && !cell->HasBomb()
        && !cell->HasPlayer();
}

// 指定したマスにプレイヤーを配置できるか確認する
bool GridManager::CanPlacePlayer(const GridPosition& position) const
{
    const GridCell* cell = GetCell(position);

    // 範囲外のマスには配置できない
    if (cell == nullptr)
    {
        return false;
    }

    // 移動可能なマスで、プレイヤーがまだいない場合に配置できる
    return cell->IsWalkable()
        && !cell->HasPlayer();
}


//==================================================
// ワールド座標との変換設定

// 1マスあたりのワールド空間でのサイズを設定する
bool GridManager::SetCellSize(float x, float y, float z)
{
    // 0以下のサイズは設定できない
    if (x <= 0.0f || y <= 0.0f || z <= 0.0f)
    {
        return false;
    }

    // 各方向の1マスのワールドサイズを保存する
    m_cellSize = DirectX::XMFLOAT3(x, y, z);

    return true;
}

// グリッドの原点となるワールド座標を設定する
void GridManager::SetGridOrigin(
    const DirectX::XMFLOAT3& origin)
{
    m_gridOrigin = origin;
}


//==================================================
// グリッド座標からワールド座標への変換

// グリッド座標をワールド座標に変換する
bool GridManager::GridToWorld(
    const GridPosition& position,
    DirectX::XMFLOAT3& worldPosition
) const
{
    // グリッドの範囲外なら変換しない
    if (!IsInside(position))
    {
        return false;
    }

    // グリッドの原点に、各座標と1マスのサイズを掛けた値を加算する
    worldPosition.x =
        m_gridOrigin.x + position.GetX() * m_cellSize.x;

    worldPosition.y =
        m_gridOrigin.y + position.GetY() * m_cellSize.y;

    worldPosition.z =
        m_gridOrigin.z + position.GetZ() * m_cellSize.z;

    return true;
}


//==================================================
// ワールド座標からグリッド座標への変換

// ワールド座標を最も近いグリッド座標に変換する
// 変換先がグリッドの範囲外の場合はfalseを返す
bool GridManager::WorldToGrid(
    const DirectX::XMFLOAT3& worldPosition,
    GridPosition& gridPosition
) const
{
    // グリッドが初期化されていない場合は変換できない
    if (m_cells.empty())
    {
        return false;
    }


    // ワールド座標からグリッド座標を計算

    // ワールド原点との差を1マスのサイズで割り、グリッド座標を求める
    // doubleを使用して計算精度を確保する
    double x =
        (static_cast<double>(worldPosition.x) - m_gridOrigin.x)
        / m_cellSize.x;

    double y =
        (static_cast<double>(worldPosition.y) - m_gridOrigin.y)
        / m_cellSize.y;

    double z =
        (static_cast<double>(worldPosition.z) - m_gridOrigin.z)
        / m_cellSize.z;


    // 計算結果の確認

    // NaNや無限大など、正常な数値でない場合は変換しない
    if (!std::isfinite(x) ||
        !std::isfinite(y) ||
        !std::isfinite(z))
    {
        return false;
    }

    // 最も近いグリッド座標に丸める
    x = std::round(x);
    y = std::round(y);
    z = std::round(z);

    // intで扱える範囲か確認する
    if (x < INT_MIN || x > INT_MAX ||
        y < INT_MIN || y > INT_MAX ||
        z < INT_MIN || z > INT_MAX)
    {
        return false;
    }


    // グリッド座標への変換

    // 計算結果を整数に変換してGridPositionを作成する
    GridPosition result(
        static_cast<int>(x),
        static_cast<int>(y),
        static_cast<int>(z)
    );

    // 変換した座標がグリッドの範囲内か確認する
    if (!IsInside(result))
    {
        return false;
    }

    // 範囲内であることを確認できたら、呼び出し元に結果を返す
    gridPosition = result;

    return true;
}