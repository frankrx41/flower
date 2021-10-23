#pragma once

typedef struct SceneManager SceneManager;
typedef struct EventInfo EventInfo;
typedef struct Scene Scene;
typedef struct Actor Actor;
typedef struct vec2 vec2;
typedef union tdata tdata;
typedef enum Event Event;

typedef bool (*CB_FindData_Bool_tPtr_tPtr)      (const tptr data, const tptr ptr);
typedef void (*CB_ActorCreate_Void_Actor_tPtr)  (Actor* actor, const tptr ptr);
typedef void (*CB_SceneDestroy_Void_Scene)      (Scene* scene);

Scene*  Scene_Create                (const strcrc* local_name, SceneManager* scene_manager, CB_SceneDestroy_Void_Scene cb_scene_destroy_void_scene);
void    Scene_Destroy               (Scene* scene);
void    Scene_Render_Offset_Set     (Scene* scene, vec2 vec);
vec2    Scene_Render_Offset_Get     (Scene* scene);
void    Scene_Pause_All             (Scene* scene);
bool    Scene_Is_Hide               (Scene* scene);
void    Scene_Hide                  (Scene* scene, bool is_pause);
bool    Scene_Is_Pause              (Scene* scene);
void    Scene_Pause                 (Scene* scene, bool is_pause);

const strcrc*    Scene_LocalName_Str_Get (Scene* scene);

Actor*  Scene_Actor_Create          (const strcrc* local_name, Scene* scene, CB_ActorCreate_Void_Actor_tPtr cb_actor_create_void_actor_tptr, tptr ptr);
void    Scene_Actor_Destroy         (Scene* scene, CB_FindData_Bool_tPtr_tPtr cb_find_actor_bool_tptr_tptr, tptr ptr);
void    Scene_Actor_Destroy_All     (Scene* scene);

void    Scene_SceneEvent_SendTo_Actor   (Scene* scene, EventInfo* event_info);
void    Scene_ControlEvent_SendTo_Actor (Scene* scene, EventInfo* event_info);

void    Scene_SceneEventGroup_Actor_Add     (Scene* scene, Actor* actor, Event event);
void    Scene_ControlEventGroup_Actor_Add   (Scene* scene, Actor* actor, Event event);
void    Scene_PhysicsGroup_Actor_Add        (Scene* scene, Actor* actor);
void    Scene_PhysicsGroup_Actor_Remove     (Scene* scene, Actor* actor);
void    Scene_PhysicsGroup_Actor_Update     (Scene* scene, float delta_seconds);


void    Scene_Storage_StoreData         (Scene* scene, crc32 variable, tdata data);
bool    Scene_Storage_IsExistVariable   (Scene* scene, crc32 variable);
tdata   Scene_Storage_ReadData          (Scene* scene, crc32 variable);
void    Scene_Storage_DeleteVariable    (Scene* scene, crc32 variable);

tptr    Scene_ActorQueue_Child_Get      (Scene* scene);
tptr    Scene_ActorQueue_Renderable_Get (Scene* scene);
void    Scene_ActorQueue_Renderable_Add     (Scene* scene, Actor* actor);
void    Scene_ActorQueue_Renderable_Remove  (Scene* scene, Actor* actor);
