#pragma once

typedef struct Sence Sence;
typedef struct Actor Actor;
typedef enum Event Event;


Sence*  Sence_Create    (const tchar* local_name);
void    Sence_Destroy   (Sence* sence);

Actor*  Sence_Actor_Create      (const tchar* local_name, Sence* sence);
void    Sence_Actor_Destroy     (Sence* sence, FindDataFunc find_actor_func, tptr ptr);
void    Sence_Actor_SendEvent   (Sence* sence, Event event);

tptr    Sence_GetActorQueue     (Sence* sence);
