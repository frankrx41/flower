#pragma once

typedef struct Storage Storage;
typedef union data32 data32;

/*
Storage will cache the variable that pass in `Storage_IsExistVariable`
You can use `Storage_IsExistVariable` before `Storage_ReadXXX`
*/

Storage*    Storage_Create          (const tchar* local_name);
void        Storage_Destroy         (Storage* storage);

void        Storage_StoreData32     (Storage* storage, crc32 variable, data32 data);
bool        Storage_IsExistVariable (Storage* storage, crc32 variable);
data32      Storage_ReadData32      (const Storage* storage, crc32 variable);
void        Storage_DeleteVariable  (Storage* storage, crc32 variable);

uint32      Storage_GetSotreCount   (const Storage* storage);
