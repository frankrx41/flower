#pragma once

typedef struct Sence Sence;
typedef struct Actor Actor;
typedef enum Event Event;
typedef struct EventInfo EventInfo;

typedef bool (*CB_FindData)(tptr data, tptr ptr);



Actor*  Sence_Actor_Create          (const tchar* local_name, Sence* sence);
void    Sence_Actor_Destroy         (Sence* sence, CB_FindData cb_find_actor, tptr ptr);
void    Sence_Actor_SendEvent       (Sence* sence, EventInfo* event_info);
void    Sence_Actor_AddEventGroup   (Sence* sence, Actor* actor, Event event);

tptr    Sence_GetActorQueue         (Sence* sence);
