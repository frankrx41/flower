#include "CoreMini.h"

#include "Queue.h"
#include "tData.h"
#include "StorageComponent.h"

#include "MemoryManager.h"
#include "Storage.h"

struct StorageComponent
{
    Storage* m_storage;
};


StorageComponent* Component_Storage_Create(const strcrc* local_name, Actor* actor)
{
    StorageComponent* storage_component = MemNew(local_name, StorageComponent);
    storage_component->m_storage        = Storage_Create(local_name);
    return storage_component;
}

void Component_Storage_Destroy(StorageComponent* storage_component)
{
    Assert(storage_component, "");
    Storage_Destroy(storage_component->m_storage);
    MemDel(storage_component);
}

bool Component_Storage_Is_ExistVariable(StorageComponent* storage_component, crc32 variable)
{
    Assert(storage_component, "");
    return
    Storage_Is_ExistVariable(storage_component->m_storage, variable);
}

void Component_Storage_Data_Store(StorageComponent* storage_component, crc32 variable, tdata data)
{
    Assert(storage_component, "");
    Storage_Data_Store(storage_component->m_storage, variable, data);
}

tdata Component_Storage_Data_Read(StorageComponent* storage_component, crc32 variable)
{
    Assert(storage_component, "");
    return
    Storage_Data_Read(storage_component->m_storage, variable);
}

void Component_Storage_Variable_Delete(StorageComponent* storage_component, crc32 variable)
{
    Assert(storage_component, "");
    Storage_Variable_Delete(storage_component->m_storage, variable);
}
