#pragma once

typedef struct StorageComponent StorageComponent;
typedef union data32 data32;

StorageComponent*   Component_Storage_Create            (const tchar* local_name);
void                Component_Storage_Destroy           (StorageComponent* storage_component);

bool                Component_Storage_IsExistVariable   (StorageComponent* storage_component, crc32 variable);
void                Component_Storage_StoreData32       (StorageComponent* storage_component, crc32 variable, data32 data);
data32              Component_Storage_ReadData32        (StorageComponent* storage_component, crc32 variable);
void                Component_Storage_DeleteVariable    (StorageComponent* storage_component, crc32 variable);
