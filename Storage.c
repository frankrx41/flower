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
    StoreType_Pointer,
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
    String*         m_local_name;
    Queue(NULL)*    m_store_queue;
};

Storage* Storage_Create(const tchar* local_name)
{
    Storage* storage = MemNew(local_name, Storage);
    storage->m_store_queue  = Queue_Create(local_name, StoreContent*);
    storage->m_local_name   = String_New(local_name);
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

void Storage_StorePointer(Storage* storage, crc32 variable, tptr value, tsize size)
{
    Assert(false, "I not sure we need this or not");
}

uint32 Storage_GetSotreCnt(const Storage* storage)
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
    StoreContent* store_content = Queue_IsExist(storage->m_store_queue, (FindDataFunc)Storage_FindVariable, (tptr)variable);
    if( store_content )
    {
        return true;
    }
    return false;
}

int32 Storage_LoadInt32(Storage* storage, crc32 variable)
{
    StoreContent* store_content = Queue_RemoveFrist(StoreContent*)(storage->m_store_queue, (FindDataFunc)Storage_FindVariable, (tptr)variable);
    Assert(store_content != NULL, "You try to read a not exist var");

    int32 data = store_content->m_int32;
    MemDel(store_content);

    return data;
}

float Storage_LoadFloat(Storage* storage, crc32 variable)
{
    StoreContent* store_content = Queue_RemoveFrist(StoreContent*)(storage->m_store_queue, (FindDataFunc)Storage_FindVariable, (tptr)variable);
    Assert(store_content != NULL, "");

    float data = store_content->m_float;
    MemDel(store_content);

    return data;
}

tptr Storage_LoadPointer(Storage* storage, crc32 variable)
{
    Assert(false, "I not sure we need this or not");
    return NULL;
}

