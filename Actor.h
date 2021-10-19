#pragma once

typedef struct Actor Actor;
typedef struct Scene Scene;
typedef enum Component Component;

typedef void (*CB_ActorCreate_Void_Actor_tPtr)  (Actor* actor, tptr ptr);
typedef void (*CB_ActorDestroy_Void_Actor)      (Actor* actor);

void    Actor_Destroy_CB_Set    (Actor* actor, CB_ActorDestroy_Void_Actor cb_actor_destroy_void_actor);
void    Actor_Destroy           (Actor* actor);

bool    Actor_Is_Pause          (Actor* actor);
void    Actor_Pause             (Actor* actor, bool is_pause);
bool    Actor_Is_Hide           (Actor* actor);
void    Actor_Hide              (Actor* actor, bool is_hide);

/*
Actor_Component_Del will be call when Actor_Destroy, you cannot call it.
*/
void    Actor_Component_New     (Actor* actor, Component component_enum);
void    Actor_Component_Del     (Actor* actor, Component component_enum);
tptr    Actor_Component_Cast    (Actor* actor, Component component_enum);

const tchar*    Actor_LocalName_Str_Get     (Actor* actor);
Scene*          Actor_OwnerScene_Get       (Actor* actor);

