#pragma once

typedef struct Scene Scene;
typedef struct Actor Actor;
typedef enum Event Event;
typedef struct EventInfo EventInfo;
typedef union data32 data32;

typedef bool (*CB_FindData_Bool_tPtr_tPtr)      (const tptr data, const tptr ptr);
typedef void (*CB_ActorCreate_Void_Actor_tPtr)  (Actor* actor, const tptr ptr);

Actor*  Scene_Actor_Create          (const tchar* local_name, Scene* scene, CB_ActorCreate_Void_Actor_tPtr cb_actor_create, tptr ptr);
void    Scene_Actor_Destroy         (Scene* scene, CB_FindData_Bool_tPtr_tPtr cb_find_actor, tptr ptr);
void    Scene_Actor_Destroy_All     (Scene* scene);
void    Scene_Actor_SendEvent       (Scene* scene, EventInfo* event_info);
void    Scene_Actor_AddEventGroup   (Scene* scene, Actor* actor, Event event);

void    Scene_Storage_StoreData32       (Scene* scene, crc32 variable, data32 data);
bool    Scene_Storage_IsExistVariable   (Scene* scene, crc32 variable);
data32  Scene_Storage_ReadData32        (Scene* scene, crc32 variable);
void    Scene_Storage_DeleteVariable    (Scene* scene, crc32 variable);

tptr    Scene_GetActorQueue         (Scene* scene);
