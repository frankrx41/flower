#pragma once

typedef struct RenderManager RenderManager;
typedef struct Sence Sence;
typedef struct RenderDataText RenderDataText;


void    RenderManager_Initialize        ();
void    RenderManager_SetPlatformData   (tptr ptr);
tptr    RenderManager_GetPlatformData   ();
void    RenderManager_RenderSenceActor  (Sence* sence);
void    RenderManager_RenderToScreen    ();

RenderDataText* RenderDataText_Create       (const tchar* local_name, int32 x, int32 y, const tchar* str);
void            RenderDataText_Destory      (RenderDataText* render_data_text);
void            RenderDataText_Disable      (RenderDataText* render_data_text);
void            RenderDataText_Move         (RenderDataText* render_data_text, int32 offset_x, int32 offset_y);
void            RenderDataText_UpdateStr    (RenderDataText* render_data_text, const tchar* str);
void            RenderDataText_UpdateInfo   (RenderDataText* render_data_text, uint32 info);

RenderManager* RenderManager_GetInstance();
