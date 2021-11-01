#include "CoreMini.h"

#include "Queue.h"
#include "tData.h"
#include "StorageComponent.h"

#include "MemoryManager.h"
#include "Component.h"
#include "Storage.h"

typedef struct StorageComponentData StorageComponentData;
struct StorageComponentData
{
    Storage* m_storage;
};


void* Component_Storage_Create(const strcrc* local_name, Actor* actor)
{
    StorageComponentData* storage_component_data = MemNew(local_name, StorageComponentData);
    storage_component_data->m_storage   = Storage_Create(local_name);
    return storage_component_data;
}

void Component_Storage_Destroy(Component* component)
{
    Assert(component, "");
    Assert(Component_GetEnum(component) == Component_Storage, "");

    StorageComponentData* storage_component_data = Component_GetData(component);
    Storage_Destroy(storage_component_data->m_storage);
    MemDel(storage_component_data);
}

bool Component_Storage_Is_ExistVariable(Component* component, crc32 variable)
{
    Assert(component, "");
    Assert(Component_GetEnum(component) == Component_Storage, "");

    StorageComponentData* storage_component_data = Component_GetData(component);
    return
    Storage_Is_ExistVariable(storage_component_data->m_storage, variable);
}

void Component_Storage_Data_Store(Component* component, crc32 variable, tdata data)
{
    Assert(component, "");
    Assert(Component_GetEnum(component) == Component_Storage, "");

    StorageComponentData* storage_component_data = Component_GetData(component);
    Storage_Data_Store(storage_component_data->m_storage, variable, data);
}

tdata Component_Storage_Data_Read(Component* component, crc32 variable)
{
    Assert(component, "");
    Assert(Component_GetEnum(component) == Component_Storage, "");

    StorageComponentData* storage_component_data = Component_GetData(component);
    return
    Storage_Data_Read(storage_component_data->m_storage, variable);
}

void Component_Storage_Variable_Delete(Component* component, crc32 variable)
{
    Assert(component, "");
    Assert(Component_GetEnum(component) == Component_Storage, "");

    StorageComponentData* storage_component_data = Component_GetData(component);
    Storage_Variable_Delete(storage_component_data->m_storage, variable);
}
