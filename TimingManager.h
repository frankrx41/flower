#pragma once

typedef struct TimingManager  TimingManager;


void    TimingManager_SetFrameRate              (TimingManager* timing_manager, float fps);
float   TimingManager_GetFrameRate              (TimingManager* timing_manager);
void    TimingManager_SetFrameRateLimit         (TimingManager* timing_manager, bool is_limit);
bool    TimingManager_IsLimitFrameRate          (TimingManager* timing_manager);
float   TimingManager_GetPrevFrameDeltaSeconds  (TimingManager* timing_manager);

void    TimingManager_TrimSpeed                 (TimingManager* timing_manager);
float   TimingManager_Cpu_Seconds_Get           (TimingManager* timing_manager);

void    TimingManager_ResetIdleSecond           (TimingManager* timing_manager);

TimingManager* TimingManager_GetInstance        ();
