#pragma once


void    Engine_Initialize   ();
void    Engine_MainLoop     ();
void    Engine_UnInitialize ();
void    Engine_SetExit      (bool is_exit);
bool    Engine_IsExit       ();

void    Engine_Debug_Memory_AllocSize_Add   (int32 size);
tsize   Engine_Debug_Memory_AllocSize_Get   ();
