#pragma once

#include "Queue.h"

typedef struct Storage Storage;

Storage*    Storage_Create          (const tchar* local_name);
void        Storage_Destroy         (Storage* storage);

void        Storage_StoreInt32      (Storage* storage, crc32 variable, int32 value);
void        Storage_StoreFloat      (Storage* storage, crc32 variable, float value);
void        Storage_StoreStruct     (Storage* storage, crc32 variable, tptr value);

uint32      Storage_GetSotreCount   (const Storage* storage);

bool        Storage_IsExistVariable (const Storage* storage, crc32 variable);
int32       Storage_LoadInt32       (const Storage* storage, crc32 variable);
float       Storage_LoadFloat       (const Storage* storage, crc32 variable);
tptr        Storage_LoadStruct      (const Storage* storage, crc32 variable);

tptr        Storage_DeleteVariable  (Storage* storage, crc32 variable);

