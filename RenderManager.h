#pragma once

typedef struct RenderManager RenderManager;
typedef struct Sence Sence;


void    RenderManager_Initialize        ();
void    RenderManager_SetPlatformData   (tptr ptr);
tptr    RenderManager_GetPlatformData   ();
void    RenderManager_RenderSence       (Sence* sence);
void    RenderManager_RenderToScreen    ();

RenderManager* RenderManager_GetInstance();
