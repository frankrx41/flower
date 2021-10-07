#pragma once

typedef struct Actor Actor;
typedef struct Sence Sence;
typedef struct ShaderText ShaderText;
typedef enum Event Event;
typedef enum Component Component;
typedef struct vec3 vec3;

typedef tptr (*CB_ComponentCreate)(const tchar* local_name);
typedef void (*CB_ComponentDestroy)(tptr component);

Actor*  Actor_Create        (const tchar* local_name, Sence* sence, uint32 id);
void    Actor_Destroy       (Actor* actor);


void    Actor_Component_New     (Actor* actor, const tchar* component_name, Component component_enum, CB_ComponentCreate cb_component_create_func);
void    Actor_Component_Del     (Actor* actor, const tchar* component_name, Component component_enum, CB_ComponentDestroy cb_component_destroy_func);
tptr    Actor_Component_Cast    (Actor* actor, const tchar* component_name, Component component_enum);

const tchar*    Actor_GetLocalName  (Actor* actor);
Sence*          Actor_GetSence      (Actor* actor);

/*
Actor_Component_Del will be call when Actor_Destroy, you can not del it.
*/

#define Actor_Component_New(actor, component)   Actor_Component_New(actor, MACRO_TOSTR(component), component, MACRO_CONNNECT(component,_Create))
#define Actor_Component_Del(actor, component)   Actor_Component_Del(actor, MACRO_TOSTR(component), component, MACRO_CONNNECT(component,_Destroy))
#define Actor_Component_Cast(actor, component)  (tptr)Actor_Component_Cast(actor, MACRO_TOSTR(component), component)

