#pragma once

typedef struct Actor Actor;
typedef struct Scene Scene;
typedef struct Component Component;
typedef struct strcrc strcrc;
typedef enum ComponentEnum ComponentEnum;

typedef void (*CB_ActorCreate_Void_Actor_tPtr)  (Actor* actor, const void* ptr);
typedef void (*CB_ActorDestroy_Void_Actor)      (Actor* actor);

void    Actor_Destroy           (Actor* actor);

bool    Actor_IsPause           (Actor* actor);
void    Actor_Pause             (Actor* actor, bool is_pause);
bool    Actor_IsHide            (Actor* actor);
void    Actor_Hide              (Actor* actor, bool is_hide);

void        Actor_Component_New     (Actor* actor, ComponentEnum component_enum);
void        Actor_Component_Del     (Actor* actor, ComponentEnum component_enum);
Component*  Actor_Component_Cast    (const Actor* actor, ComponentEnum component_enum);


const strcrc*   Actor_GetLocalName      (Actor* actor);
Scene*          Actor_GetExistScene     (const Actor* actor);

