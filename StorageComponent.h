#pragma once

typedef struct StorageComponent StorageComponent;
typedef union tdata tdata;
typedef struct Actor Actor;
typedef struct strcrc strcrc;


StorageComponent*   Component_Storage_Create            (const strcrc* local_name, Actor* actor);
void                Component_Storage_Destroy           (StorageComponent* storage_component);

bool                Component_Storage_Is_ExistVariable  (StorageComponent* storage_component, crc32 variable);
void                Component_Storage_Data_Store        (StorageComponent* storage_component, crc32 variable, tdata data);
tdata               Component_Storage_Data_Read         (StorageComponent* storage_component, crc32 variable);
void                Component_Storage_Variable_Delete   (StorageComponent* storage_component, crc32 variable);
