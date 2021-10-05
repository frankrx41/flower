#pragma once

typedef struct Storage Storage;

/*
Storage will cache the variable that pass in `Storage_IsExistVariable`
You can use `Storage_IsExistVariable` before `Storage_ReadXXX`
*/

Storage*    Storage_Create          (const tchar* local_name);
void        Storage_Destroy         (Storage* storage);

void        Storage_StoreInt32      (Storage* storage, crc32 variable, int32 value);
void        Storage_StoreFloat      (Storage* storage, crc32 variable, float value);
void        Storage_StorePointer    (Storage* storage, crc32 variable, tptr value);

uint32      Storage_GetSotreCount   (const Storage* storage);

bool        Storage_IsExistVariable (Storage* storage, crc32 variable);
int32       Storage_ReadInt32       (const Storage* storage, crc32 variable);
float       Storage_ReadFloat       (const Storage* storage, crc32 variable);
tptr        Storage_ReadPointer     (const Storage* storage, crc32 variable);

void        Storage_DeleteVariable  (Storage* storage, crc32 variable);

