#pragma once

typedef struct Actor Actor;
typedef struct Sence Sence;
typedef struct RenderDataText RenderDataText;
typedef enum Event Event;
typedef enum Component Component;


typedef void (*ActorActionFunc)(Actor*);

typedef tptr (*CB_ComponentCreateFunc)(const tchar* local_name);
typedef void (*CB_ComponentDestroyFunc)(tptr component);



Actor*  Actor_Create        (const tchar* local_name, Sence* sence, uint32 id);
void    Actor_Destroy       (Actor* actor);

void    Actor_RegisterEvent     (Actor* actor, Event event, ActorActionFunc actor_action_func);
void    Actor_UnregisterEvent   (Actor* actor, Event event);
void    Actor_ProcessEvent      (Actor* actor, Event event);

RenderDataText*     Actor_Component_Render_RenderDataText_Add        (Actor* actor, uint32 x, uint32 y, const tchar* str);
void                Actor_Component_Render_RenderDataText_ClearAll   (Actor* actor);


void    Actor_Component_New     (Actor* actor, const tchar* component_name, Component component_enum, CB_ComponentCreateFunc cb_component_create_func);
void    Actor_Component_Del     (Actor* actor, const tchar* component_name, Component component_enum, CB_ComponentDestroyFunc cb_component_destroy_func);
tptr    Actor_Component_Cast    (Actor* actor, const tchar* component_name, Component component_enum);

const tchar* Actor_GetLocalName (Actor* actor);

// #define Actor_Component_Add(actor, component)   Actor_Component_Add(actor, MACRO_TOSTR(component), MACRO_CONNNECT(MACRO_CONNNECT(Component_,component),_Create)(Actor_GetLocalName(actor)))

#define Actor_Component_New(actor, component)   Actor_Component_New(actor, MACRO_TOSTR(component), component, MACRO_CONNNECT(component,_Create))
#define Actor_Component_Del(actor, component)   Actor_Component_Del(actor, MACRO_TOSTR(component), component, MACRO_CONNNECT(component,_Destroy))
#define Actor_Component_Cast(actor, component)  (tptr)Actor_Component_Cast(actor, MACRO_TOSTR(component), component)

