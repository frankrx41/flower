#pragma once

typedef struct vec3 vec3;
typedef struct Actor Actor;
typedef struct ShaderText ShaderText;
typedef struct EventInfo EventInfo;
typedef void (*CB_EventRespond_Void_Actor_EventInfo)(Actor*, const EventInfo*);
typedef enum Event Event;
typedef union tdata tdata;


void    Actor_Component_Location_Set    (Actor* actor, vec3 vec);
void    Actor_Component_Location_Move   (Actor* actor, vec3 offset_vec);
vec3    Actor_Component_Location_Get    (Actor* actor);


ShaderText* Actor_Component_Render_ShaderText_Add       (Actor* actor, vec3 vec, const tchar* str);
void        Actor_Component_Render_ShaderText_Del       (Actor* actor, ShaderText* shader_text);
void        Actor_Component_Render_ShaderText_ClearAll  (Actor* actor);


void    Actor_Component_Action_EventRespond_Add     (Actor* actor, Event event, CB_EventRespond_Void_Actor_EventInfo cb_event_respond);
void    Actor_Component_Action_EventRespond_Del     (Actor* actor, Event event);
void    Actor_Component_Action_EventRespond_Clear   (Actor* actor);


bool    Actor_Component_Storage_IsExistVariable (Actor* actor, crc32 variable);
void    Actor_Component_Storage_StoreData       (Actor* actor, crc32 variable, tdata data);
tdata   Actor_Component_Storage_ReadData        (Actor* actor, crc32 variable);
void    Actor_Component_Storage_DeleteVariable  (Actor* actor, crc32 variable);
