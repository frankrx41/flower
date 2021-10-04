#pragma once

typedef struct RenderContent RenderContent;
typedef struct RenderManager RenderManager;
typedef struct Sence Sence;
typedef struct RenderData RenderData;


void    RenderManager_Initialize        ();
void    RenderManager_SetPlatformData   (tptr ptr);
tptr    RenderManager_GetPlatformData   ();
void    RenderManager_RenderSenceActor  (Sence* sence);
void    RenderManager_RenderToScreen    ();

RenderManager* RenderManager_GetInstance();

void RenderContent_Render_Plat(RenderContent* render_content ,uint32 x, uint32 y, const tchar* str);
