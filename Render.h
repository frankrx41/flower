#pragma once

typedef struct RenderManager RenderManager;
typedef struct Sence Sence;
typedef struct RenderData2D RenderData2D;


void    RenderManager_Initialize        ();
void    RenderManager_SetPlatformData   (tptr ptr);
tptr    RenderManager_GetPlatformData   ();
void    RenderManager_RenderSenceActor  (Sence* sence);
void    RenderManager_RenderToScreen    ();

RenderData2D*   RenderData2D_Create     (int32 x, int32 y, const tchar* str);
void            RenderData2D_Destory    (RenderData2D* render_data_2d);

RenderManager* RenderManager_GetInstance();
