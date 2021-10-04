#pragma once

#include "Queue.h"

typedef struct Storage Storage;



Storage*    Storage_Create      (const tchar* local_name);
void        Storage_Destroy     (Storage* storage);

void        Storage_StoreInt32  (Storage* storage, crc32 variable, int32 value);
void        Storage_StoreFloat  (Storage* storage, crc32 variable, float value);
void        Storage_StorePointer(Storage* storage, crc32 variable, tptr value, tsize size);

uint32      Storage_GetSotreCnt (const Storage* storage);

int32       Storage_LoadInt32   (Storage* storage, crc32 variable);
float       Storage_LoadFloat   (Storage* storage, crc32 variable);
tptr        Storage_LoadPointer (Storage* storage, crc32 variable);
