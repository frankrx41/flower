#include "CoreMini.h"

#include "Queue.h"
#include "Data32.h"
#include "StorageComponent.h"

#include "MemoryManager.h"
#include "Storage.h"

struct StorageComponent
{
    Storage* m_storage;
};


StorageComponent* Component_Storage_Create(const tchar* local_name)
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

bool Component_Storage_IsExistVariable(StorageComponent* storage_component, crc32 variable)
{
    Assert(storage_component, "");
    return
    Storage_IsExistVariable(storage_component->m_storage, variable);
}

void Component_Storage_StoreData32(StorageComponent* storage_component, crc32 variable, data32 data)
{
    Assert(storage_component, "");
    Storage_StoreData32(storage_component->m_storage, variable, data);
}

data32 Component_Storage_ReadData32(StorageComponent* storage_component, crc32 variable)
{
    Assert(storage_component, "");
    return
    Storage_ReadData32(storage_component->m_storage, variable);
}

void Component_Storage_DeleteVariable(StorageComponent* storage_component, crc32 variable)
{
    Assert(storage_component, "");
    Storage_DeleteVariable(storage_component->m_storage, variable);
}
