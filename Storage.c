#include "CoreMini.h"

#include "Queue.h"
#include "Storage.h"

#include "Memory.h"
#include "String.h"

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
    StoreType   m_store_type;
    union
    {
        int32   m_int32;
        float   m_float;
        tptr    m_pointer;
    };
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
    storage->m_local_name   = String_New(local_name);
    storage->m_cache_store_content = NULL;
    return storage;
}

void Storage_Destroy(Storage* storage)
{
    Queue_Destroy(storage->m_store_queue);
    String_Del(storage->m_local_name);
    MemDel(storage);
}

void Storage_StoreInt32(Storage* storage, crc32 variable, int32 value)
{
    StoreContent* store_content = MemNew(String_CStr(storage->m_local_name), StoreContent);
    store_content->m_crc        = variable;
    store_content->m_store_type = StoreType_Int32;
    store_content->m_int32      = value;
    
    Queue_Push(StoreContent*, storage->m_store_queue, store_content);
}

void Storage_StoreFloat(Storage* storage, crc32 variable, float value)
{
    StoreContent* store_content = MemNew(String_CStr(storage->m_local_name), StoreContent);
    store_content->m_crc        = variable;
    store_content->m_store_type = StoreType_Int32;
    store_content->m_float      = value;
    
    Queue_Push(StoreContent*, storage->m_store_queue, store_content);
}

void Storage_StorePointer(Storage* storage, crc32 variable, tptr value)
{
    StoreContent* store_content = MemNew(String_CStr(storage->m_local_name), StoreContent);
    store_content->m_crc        = variable;
    store_content->m_store_type = StoreType_Struct;
    store_content->m_pointer    = value;

    Queue_Push(StoreContent*, storage->m_store_queue, store_content);
}

uint32 Storage_GetSotreCount(const Storage* storage)
{
    return
    Queue_GetLength(storage->m_store_queue);
}

static bool Storage_FindVariable(StoreContent* store_content, crc32 variable)
{
    if(store_content->m_crc == variable)
    {
        return true;
    }
    return false;
}

bool Storage_IsExistVariable(Storage* storage, crc32 variable)
{
    StoreContent* store_content = Queue_Find(StoreContent*)(storage->m_store_queue, (FindDataFunc)Storage_FindVariable, (tptr)variable);
    if( store_content )
    {
        storage->m_cache_store_content = store_content;
        return true;
    }
    return false;
}

static StoreContent* Storage_FindStoreContent(const Storage* storage, crc32 variable)
{
    // Add cache
    if( storage->m_cache_store_content && storage->m_cache_store_content->m_crc == variable )
    {
        return storage->m_cache_store_content;
    }

    StoreContent* store_content = Queue_Find(StoreContent*)(storage->m_store_queue, (FindDataFunc)Storage_FindVariable, (tptr)variable);
    // Assert(store_content != NULL, "You try to read a not exist var");

    return store_content;
}

int32 Storage_LoadInt32(const Storage* storage, crc32 variable)
{
    StoreContent* store_content = Storage_FindStoreContent(storage, variable);
    return store_content ? store_content->m_int32 : 0;
}

float Storage_LoadFloat(const Storage* storage, crc32 variable)
{
    StoreContent* store_content = Storage_FindStoreContent(storage, variable);
    return store_content ? store_content->m_float : 0.f;
}

tptr Storage_LoadPointer(const Storage* storage, crc32 variable)
{
    StoreContent* store_content = Storage_FindStoreContent(storage, variable);
    return store_content ? store_content->m_pointer : NULL;
}

void Storage_DeleteVariable(Storage* storage, crc32 variable)
{
    StoreContent* store_content = Storage_FindStoreContent(storage, variable);
    MemDel(store_content);
}