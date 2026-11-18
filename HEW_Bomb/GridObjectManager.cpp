#include "GridObjectManager.h"

#include "GridObject.h"

#include "Wall.h"
#include "NetworkServer.h"
#include "Generator.h"
#include "Computer.h"
#include "Pillar.h"
#include "OilCan.h"
#include "Floor.h"

#include <utility>
#include <algorithm>

GridObjectManager::GridObjectManager()
    : m_hasSavedState(false)
{
}

GridObjectManager::~GridObjectManager()
{
    Clear();
}

// オブジェクトを生成して登録
bool GridObjectManager::CreateObject(
    GridObjectType type,
    const GridPosition& position)
{
    // 床と通常オブジェクトを別々に管理する
    if (type == GridObjectType::Floor)
    {
        // 同じ位置に床が存在する場合は登録しない
        if (GetFloorAt(position) != nullptr)
        {
            return false;
        }
    }
    else
    {
        // 同じ位置に通常オブジェクトが存在する場合は登録しない
        if (GetObjectAt(position) != nullptr)
        {
            return false;
        }
    }

    std::unique_ptr<GridObject> object;

    // オブジェクトの種類に応じて生成する
    switch (type)
    {
    case GridObjectType::Wall:
        object.reset(new Wall(position));
        break;

    case GridObjectType::NetworkServer:
        object.reset(new NetworkServer(position));
        break;

    case GridObjectType::Generator:
        object.reset(new Generator(position));
        break;

    case GridObjectType::Computer:
        object.reset(new Computer(position));
        break;

    case GridObjectType::Pillar:
        object.reset(new Pillar(position));
        break;

    case GridObjectType::OilCan:
        object.reset(new OilCan(position));
        break;

    case GridObjectType::Floor:
        object.reset(new Floor(position));
        break;

    default:
        return false;
    }

    // 生成したオブジェクトを登録
    m_objects.push_back(std::move(object));

    return true;
}

// 指定した位置のオブジェクトを取得
GridObject* GridObjectManager::GetObjectAt(
    const GridPosition& position)
{
    for (const std::unique_ptr<GridObject>& object : m_objects)
    {
        if (object->GetPosition() == position &&
            object->GetType() != GridObjectType::Floor)
        {
            return object.get();
        }
    }

    return nullptr;
}

// 指定した位置のオブジェクトを取得（読み取り専用）
const GridObject* GridObjectManager::GetObjectAt(
    const GridPosition& position) const
{
    for (const std::unique_ptr<GridObject>& object : m_objects)
    {
        if (object->GetPosition() == position &&
            object->GetType() != GridObjectType::Floor)
        {
            return object.get();
        }
    }

    return nullptr;
}

// 管理しているオブジェクトの数を取得
int GridObjectManager::GetObjectCount() const
{
    return static_cast<int>(m_objects.size());
}

// 破壊されたオブジェクトを削除
void GridObjectManager::RemoveDestroyedObjects()
{
    m_objects.erase(
        std::remove_if(
            m_objects.begin(),
            m_objects.end(),
            [](const std::unique_ptr<GridObject>& object)
            {
                return object->IsDestroyed();
            }),
        m_objects.end()
    );
}


// 指定した位置の床オブジェクトを取得
GridObject* GridObjectManager::GetFloorAt(
    const GridPosition& position)
{
    for (const std::unique_ptr<GridObject>& object : m_objects)
    {
        if (object->GetPosition() == position &&
            object->GetType() == GridObjectType::Floor)
        {
            return object.get();
        }
    }

    return nullptr;
}

// 指定した位置の床オブジェクトを取得（読み取り専用）
const GridObject* GridObjectManager::GetFloorAt(
    const GridPosition& position) const
{
    for (const std::unique_ptr<GridObject>& object : m_objects)
    {
        if (object->GetPosition() == position &&
            object->GetType() == GridObjectType::Floor)
        {
            return object.get();
        }
    }

    return nullptr;
}

void GridObjectManager::ProcessExplosion(
    const std::vector<GridPosition>& positions)
{
    for (const GridPosition& position : positions)
    {
        GridObject* object = GetObjectAt(position);

        if (object != nullptr)
        {
            object->OnExplosion();
        }

        GridObject* floor = GetFloorAt(position);

        if (floor != nullptr)
        {
            floor->OnExplosion();
        }
    }

   // RemoveDestroyedObjects();
}

// 全オブジェクトを解放
void GridObjectManager::Clear()
{
    m_objects.clear();
    DiscardUndoState();
}


//==================================================
// Undo用の状態を保存

bool GridObjectManager::SaveUndoState()
{
    // 古い保存データを削除
    m_savedObjects.clear();

    // 現在のオブジェクトを複製
    for (const std::unique_ptr<GridObject>& object : m_objects)
    {
        std::unique_ptr<GridObject> clone = object->Clone();

        if (clone == nullptr)
        {
            m_savedObjects.clear();
            m_hasSavedState = false;
            return false;
        }

        m_savedObjects.push_back(std::move(clone));
    }

    m_hasSavedState = true;

    return true;
}


//==================================================
// 保存した状態に戻す

bool GridObjectManager::RestoreUndoState()
{
    if (!m_hasSavedState)
    {
        return false;
    }

    // 現在のオブジェクトを保存時の状態に置き換える
    m_objects = std::move(m_savedObjects);

    m_savedObjects.clear();
    m_hasSavedState = false;

    return true;
}


//==================================================
// Undo用の保存データを破棄

void GridObjectManager::DiscardUndoState()
{
    m_savedObjects.clear();
    m_hasSavedState = false;
}