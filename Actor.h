#pragma once

typedef struct Actor Actor;
typedef struct Sence Sence;
typedef enum Event Event;


typedef void (*ActorActionFunc)(Actor*);


Actor*  Actor_Create        (const tchar* local_name, Sence* sence, uint32 id);
void    Actor_Destroy       (Actor* actor);

void    Actor_RegisterEvent     (Actor* actor, Event event, ActorActionFunc actor_action_func);
void    Actor_UnregisterEvent   (Actor* actor, Event event);
void    Actor_ProcessEvent      (Actor* actor, Event event);

void    Actor_AddComponent      (Actor* actor, const tchar* component_name, tsize size);
void    Actor_DelComponent      (Actor* actor, const tchar* component_name);
tptr    Actor_Cast              (Actor* actor, const tchar* component_name);


#define Actor_AddComponent(actor, component)    Actor_AddComponent(actor, MACRO_TOSTR(component), sizeof(component))
#define Actor_Cast(actor, component)            (component*)Actor_Cast(actor, MACRO_TOSTR(component))

