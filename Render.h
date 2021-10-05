#pragma once

typedef struct RenderManager RenderManager;
typedef struct Sence Sence;
typedef struct RenderData RenderData;


void    RenderManager_Initialize        ();
void    RenderManager_SetPlatformData   (tptr ptr);
tptr    RenderManager_GetPlatformData   ();
void    RenderManager_RenderSenceActor  (Sence* sence);
void    RenderManager_RenderToScreen    ();

RenderData* RenderData_Create           (int32 size, ...);
void        RenderData_Destory          (RenderData* render_data);

RenderManager* RenderManager_GetInstance();
