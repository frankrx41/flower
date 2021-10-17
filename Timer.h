#pragma once

typedef struct Timer Timer;

Timer*  Timer_Create            (const tchar* local_name);
void    Timer_Destroy           (Timer* timer);
float   Timer_Elapsed_Time_Get  (Timer* timer);
