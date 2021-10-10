#pragma once

typedef struct Storage Storage;
typedef union tdata tdata;

/*
Storage will cache the variable that pass in `Storage_IsExistVariable`
You can use `Storage_IsExistVariable` before `Storage_ReadXXX`
*/

Storage*    Storage_Create          (const tchar* local_name);
void        Storage_Destroy         (Storage* storage);

void        Storage_StoreData       (Storage* storage, crc32 variable, tdata data);
bool        Storage_IsExistVariable (Storage* storage, crc32 variable);
tdata       Storage_ReadData        (const Storage* storage, crc32 variable);
void        Storage_DeleteVariable  (Storage* storage, crc32 variable);

uint32      Storage_GetSotreCount   (const Storage* storage);
