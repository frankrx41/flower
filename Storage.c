#include "CoreMini.h"

#include "MemoryManager.h"

#include "Queue.h"
#include "Storage.h"
#include "String.h"
#include "tData.h"


typedef enum StoreType StoreType;

enum StoreType
{
    StoreType_Int32,
    StoreType_Float,
    StoreType_Struct,
};

typedef struct StoreContent StoreContent;

struct StoreContent
{
    crc32       m_crc;
    tdata       m_data;
};

struct Storage
{
    String*                 m_local_name;
    Queue(StoreContent*)*   m_store_queue;
    StoreContent*           m_cache_store_content;
};

Storage* Storage_Create(const tchar* local_name)
{
    Storage* storage = MemNew(local_name, Storage);
    storage->m_store_queue  = Queue_Create(local_name, StoreContent*);
    storage->m_local_name   = String_New(local_name, local_name, true);
    storage->m_cache_store_content = NULL;
    return storage;
}

static void CallBack_Storage_StoreContent_Delete(StoreContent* store_content)
{
    MemDel(store_content);
}

void Storage_Destroy(Storage* storage)
{
    String_Del(storage->m_local_name);
    Queue_Destroy(storage->m_store_queue, CallBack_Storage_StoreContent_Delete);
    MemDel(storage);
}

static bool CallBack_Storage_FindVariable(StoreContent* store_content, crc32 variable)
{
    return store_content->m_crc == variable;
}

static StoreContent* Storage_FindStoreContent(const Storage* storage, crc32 variable)
{
    // Add cache
    if( storage->m_cache_store_content && storage->m_cache_store_content->m_crc == variable )
    {
        return storage->m_cache_store_content;
    }

    StoreContent* store_content = Queue_Find(StoreContent*)(storage->m_store_queue, (CB_FindData_Bool_tPtr_tPtr)CallBack_Storage_FindVariable, (tptr)variable);
    // Assert(store_content != NULL, "You try to read a not exist var");

    return store_content;
}

void Storage_Data_Store(Storage* storage, crc32 variable, tdata data)
{
    if( Storage_Is_ExistVariable(storage, variable) )
    {
        // Assert(false, "You try to add a exist variable!");
        StoreContent* store_content = Storage_FindStoreContent(storage, variable);
        store_content->m_data = data;
    }
    else
    {
        StoreContent* store_content = MemNew(String_CStr(storage->m_local_name), StoreContent);
        store_content->m_crc        = variable;
        store_content->m_data       = data;
        Queue_Push(StoreContent*, NULL, storage->m_store_queue, store_content);
    }
}

bool Storage_Is_ExistVariable(Storage* storage, crc32 variable)
{
    StoreContent* store_content = Queue_Find(StoreContent*)(storage->m_store_queue, (CB_FindData_Bool_tPtr_tPtr)CallBack_Storage_FindVariable, (tptr)variable);
    if( store_content )
    {
        storage->m_cache_store_content = store_content;
        return true;
    }
    return false;
}

tdata Storage_Data_Read(Storage* storage, crc32 variable)
{
    StoreContent* store_content = Storage_FindStoreContent(storage, variable);
    return store_content ? store_content->m_data : tdata_null;
}

void Storage_Variable_Delete(Storage* storage, crc32 variable)
{
    StoreContent* store_content = Storage_FindStoreContent(storage, variable);
    Queue_RemoveFindFirst(StoreContent*)(storage->m_store_queue, NULL, store_content, NULL);
    MemDel(store_content);
}

uint32 Storage_StoreCount_Get(const Storage* storage)
{
    return
    Queue_GetLength(storage->m_store_queue);
}
