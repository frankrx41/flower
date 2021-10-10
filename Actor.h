#pragma once

typedef struct Actor Actor;
typedef struct Scene Scene;
typedef enum Component Component;

typedef void (*CB_ActorCreate_Void_Actor_tPtr)  (Actor* actor, tptr ptr);
typedef void (*CB_ActorDestroy_Void_Actor)      (Actor* actor);

typedef tptr (*CB_ComponentCreate_tPtr_tChar)   (const tchar* local_name);
typedef void (*CB_ComponentDestroy_Void_tPtr)   (tptr component);

Actor*  Actor_Create            (const tchar* local_name, Scene* scene, uint32 id, CB_ActorCreate_Void_Actor_tPtr cb_actor_create_void_actor_tptr, tptr ptr);
void    Actor_Set_DestroyCB     (Actor* actor, CB_ActorDestroy_Void_Actor cb_actor_destroy_void_actor);
void    Actor_Destroy           (Actor* actor);

bool    Actor_IsPause           (Actor* actor);
void    Actor_SetIsPause        (Actor* actor, bool is_pause);

void    Actor_Component_New     (Actor* actor, Component component_enum, CB_ComponentCreate_tPtr_tChar cb_component_create_tptr_tchar);
void    Actor_Component_Del     (Actor* actor, Component component_enum, CB_ComponentDestroy_Void_tPtr cb_component_destroy_void_tptr);
tptr    Actor_Component_Cast    (Actor* actor, Component component_enum);

const tchar*    Actor_GetLocalName  (Actor* actor);
Scene*          Actor_GetScene      (Actor* actor);

/*
Actor_Component_Del will be call when Actor_Destroy, you can not del it.
*/

#define Actor_Component_New(actor, component)   Actor_Component_New(actor, component, MACRO_CONNNECT(component,_Create))
#define Actor_Component_Del(actor, component)   Actor_Component_Del(actor, component, MACRO_CONNNECT(component,_Destroy))

