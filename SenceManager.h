#pragma once

typedef struct Sence Sence;
typedef struct SenceManager SenceManager;

/*
SenceManager_Sence_Destroy will call Sence_Actor_Destroy, and then call Actor_Destroy, you no need call Sence_Actor_Destroy
*/

Sence*  SenceManager_Sence_Create           (SenceManager* sence_manager, const tchar* local_name);
Sence*  SenceManager_Sence_Destroy          (SenceManager* sence_manager, Sence* sence);
void    SenceManager_Sence_SetCurrent       (SenceManager* sence_manager, Sence* sence);
Sence*  SenceManager_Sence_GetCurrent       (SenceManager* sence_manager);

SenceManager* SenceManager_GetInstance  ();

#define SenceManager_Sence_Create(local_name)   SenceManager_Sence_Create(SenceManager_GetInstance(), local_name)
#define SenceManager_Sence_Destroy(sence)       SenceManager_Sence_Destroy(SenceManager_GetInstance(), sence)
#define SenceManager_Sence_SetCurrent(sence)    SenceManager_Sence_SetCurrent(SenceManager_GetInstance(), sence)
#define SenceManager_Sence_GetCurrent()         SenceManager_Sence_GetCurrent(SenceManager_GetInstance())
