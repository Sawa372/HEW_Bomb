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

GridObjectManager::GridObjectManager()
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

// 全オブジェクトを解放
void GridObjectManager::Clear()
{
    m_objects.clear();
}