#pragma once

typedef struct Engine   Engine;
typedef struct Sence    Sence;


void    Engine_Initialize   (Engine* engine);
void    Engine_MainLoop     (Engine* engine);
void    Engine_UnInitialize (Engine* engine);
void    Engine_SetExit      (Engine* engine, bool is_exit);
