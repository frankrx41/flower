#pragma once

typedef struct Storage Storage;
typedef union tdata tdata;

/*
Storage will cache the variable that pass in `Storage_Is_ExistVariable`
You can use `Storage_Is_ExistVariable` before `Storage_Data_Read`
*/

Storage*    Storage_Create              (const tchar* local_name);
void        Storage_Destroy             (Storage* storage);

void        Storage_Data_Store          (Storage* storage, crc32 variable, tdata data);
bool        Storage_Is_ExistVariable    (Storage* storage, crc32 variable);
tdata       Storage_Data_Read           (Storage* storage, crc32 variable);
void        Storage_Variable_Delete     (Storage* storage, crc32 variable);

uint32      Storage_StoreCount_Get      (const Storage* storage);
