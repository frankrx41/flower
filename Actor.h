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



// void                Actor_Component_Location_Set                    (Actor* actor, vec3 vec);
// void                Actor_Component_Location_Move                   (Actor* actor, vec3 offset_vec);
// vec3                Actor_Component_Location_Get                    (Actor* actor);
//

void    Actor_Component_New     (Actor* actor, const tchar* component_name, Component component_enum, CB_ComponentCreate cb_component_create_func);
void    Actor_Component_Del     (Actor* actor, const tchar* component_name, Component component_enum, CB_ComponentDestroy cb_component_destroy_func);
tptr    Actor_Component_Cast    (Actor* actor, const tchar* component_name, Component component_enum);

const tchar*    Actor_GetLocalName  (Actor* actor);
Sence*          Actor_GetSence      (Actor* actor);

// #define Actor_Component_Add(actor, component)   Actor_Component_Add(actor, MACRO_TOSTR(component), MACRO_CONNNECT(MACRO_CONNNECT(Component_,component),_Create)(Actor_GetLocalName(actor)))

#define Actor_Component_New(actor, component)   Actor_Component_New(actor, MACRO_TOSTR(component), component, MACRO_CONNNECT(component,_Create))
#define Actor_Component_Del(actor, component)   Actor_Component_Del(actor, MACRO_TOSTR(component), component, MACRO_CONNNECT(component,_Destroy))
#define Actor_Component_Cast(actor, component)  (tptr)Actor_Component_Cast(actor, MACRO_TOSTR(component), component)

