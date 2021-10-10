#pragma once

typedef struct StorageComponent StorageComponent;
typedef union tdata tdata;

StorageComponent*   Component_Storage_Create            (const tchar* local_name);
void                Component_Storage_Destroy           (StorageComponent* storage_component);

bool                Component_Storage_IsExistVariable   (StorageComponent* storage_component, crc32 variable);
void                Component_Storage_StoreData         (StorageComponent* storage_component, crc32 variable, tdata data);
tdata               Component_Storage_ReadData          (StorageComponent* storage_component, crc32 variable);
void                Component_Storage_DeleteVariable    (StorageComponent* storage_component, crc32 variable);
