#pragma once

typedef struct Sence Sence;
typedef struct Actor Actor;
typedef enum Event Event;
typedef struct EventInfo EventInfo;
typedef union data32 data32;

typedef bool (*CB_FindData)(ptr32 data, ptr32 ptr);
typedef void (*CB_ActorCreate)(Actor* actor, ptr32 ptr);

Actor*  Sence_Actor_Create          (const tchar* local_name, Sence* sence, CB_ActorCreate cb_actor_create, ptr32 ptr);
void    Sence_Actor_Destroy         (Sence* sence, CB_FindData cb_find_actor, ptr32 ptr);
void    Sence_Actor_Destroy_All     (Sence* sence);
void    Sence_Actor_SendEvent       (Sence* sence, EventInfo* event_info);
void    Sence_Actor_AddEventGroup   (Sence* sence, Actor* actor, Event event);

void    Sence_Storage_StoreData32       (Sence* sence, crc32 variable, data32 data);
bool    Sence_Storage_IsExistVariable   (Sence* sence, crc32 variable);
data32  Sence_Storage_ReadData32        (Sence* sence, crc32 variable);
void    Sence_Storage_DeleteVariable    (Sence* sence, crc32 variable);

ptr32    Sence_GetActorQueue         (Sence* sence);
