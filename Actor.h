#pragma once

typedef struct Actor Actor;
typedef struct Sence Sence;
typedef enum Event Event;
typedef void Component;
typedef struct RenderDataText RenderDataText;


typedef void (*ActorActionFunc)(Actor*);


Actor*  Actor_Create        (const tchar* local_name, Sence* sence, uint32 id);
void    Actor_Destroy       (Actor* actor);

void    Actor_RegisterEvent     (Actor* actor, Event event, ActorActionFunc actor_action_func);
void    Actor_UnregisterEvent   (Actor* actor, Event event);
void    Actor_ProcessEvent      (Actor* actor, Event event);

void    Actor_Component_Add     (Actor* actor, const tchar* component_name, Component* component);
void    Actor_Component_Del     (Actor* actor, const tchar* component_name);
tptr    Actor_Component_Cast    (Actor* actor, const tchar* component_name);

const tchar* Actor_GetLocalName (Actor* actor);


#define Actor_Component_Del(actor, component)   do{ MACRO_CONNNECT(MACRO_CONNNECT(Component_,component),_Destroy)(Actor_Component_Cast(actor, component));\
Actor_Component_Del(actor, MACRO_TOSTR(component)); }while(0)
#define Actor_Component_Add(actor, component)   Actor_Component_Add(actor, MACRO_TOSTR(component), MACRO_CONNNECT(MACRO_CONNNECT(Component_,component),_Create)(Actor_GetLocalName(actor)))
#define Actor_Component_Cast(actor, component)  (Component*)Actor_Component_Cast(actor, MACRO_TOSTR(component))

