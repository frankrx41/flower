#pragma once

typedef struct Engine   Engine;
typedef struct Sence    Sence;


void    Engine_Initialize   ();
void    Engine_MainLoop     ();
void    Engine_UnInitialize ();
void    Engine_SetExit      (bool is_exit);
