#pragma once

typedef struct vec3 vec3;
typedef struct Actor Actor;
typedef struct ShaderText ShaderText;
typedef struct EventInfo EventInfo;
typedef void (*CB_RespondAction_Void_Actor_EventInfo)(Actor*, const EventInfo*);
typedef bool (*CB_RespondCondition_Bool_Actor_EventInfo)(Actor*, const EventInfo*);
typedef enum Event Event;
typedef union tdata tdata;


vec3    Actor_Component_Physics_Location_Get        (Actor* actor);
void    Actor_Component_Physics_Location_Set        (Actor* actor, vec3 vec);
void    Actor_Component_Physics_Location_Move       (Actor* actor, vec3 offset_vec);
vec3    Actor_Component_Physics_Velocity_Get        (Actor* actor);
void    Actor_Component_Physics_Velocity_Set        (Actor* actor, vec3 velocity);
vec3    Actor_Component_Physics_Acceleration_Get    (Actor* actor);
void    Actor_Component_Physics_Acceleration_Set    (Actor* actor, vec3 acceleration);
void    CallBack_Actor_Component_Physics_Simulate   (Actor* actor, float* delta_seconds);
void    Actor_Component_Physics_SetEnableSimulate   (Actor* actor, bool is_enable_simulate);


ShaderText* Actor_Component_Render_ShaderText_Add   (Actor* actor, vec3 vec, const tchar* str);
void        Actor_Component_Render_ShaderText_Del   (Actor* actor, ShaderText* shader_text);
void        Actor_Component_Render_ShaderText_Clear (Actor* actor);


void    Actor_Component_Control_EventRespond_Add    (Actor* actor, Event event, CB_RespondCondition_Bool_Actor_EventInfo cb_respond_condition_void_actor_eventinfo, CB_RespondAction_Void_Actor_EventInfo cb_respond_action_void_actor_eventinfo);
void    Actor_Component_Control_EventRespond_Del    (Actor* actor, Event event);
void    Actor_Component_Control_EventRespond_Clear  (Actor* actor);


bool    Actor_Component_Storage_IsExistVariable     (Actor* actor, crc32 variable);
void    Actor_Component_Storage_Data_Store          (Actor* actor, crc32 variable, tdata data);
tdata   Actor_Component_Storage_Data_Read           (Actor* actor, crc32 variable);
void    Actor_Component_Storage_Variable_Delete     (Actor* actor, crc32 variable);
