#pragma once

typedef struct Actor Actor;
typedef struct Sence Sence;

typedef void (*ActorActionFunc)(Actor*);


Actor*  Actor_Create        (const tchar* local_name, Sence* sence, uint32 id);
void    Actor_Destroy       (Actor* actor);

void    Actor_RegisterEvent     (Actor* actor, Event event, ActorActionFunc actor_action_func);
void    Actor_UnregisterEvent   (Actor* actor, Event event);
void    Actor_ProcessEvent      (Actor* actor, Event event);
