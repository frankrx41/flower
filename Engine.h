#pragma once

typedef struct Engine   Engine;

// Engine
error   Engine_GetLastError ();
error   Engine_SetLastError (int32 code);
error   Engine_Initialize   ();
error   Engine_MainLoop     ();
Engine* Engine_GetInstance  ();
