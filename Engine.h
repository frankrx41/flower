#pragma once

typedef struct Engine   Engine;
typedef struct Sence    Sence;


void    Engine_Initialize   ();
void    Engine_MainLoop     ();
void    Engine_UnInitialize ();
void    Engine_Exit         ();

Sence*  Engine_Sence_GetCurrentSence(Engine* engine);
void    Engine_Sence_SetCurrentSence(Engine* engine, Sence* sence);

