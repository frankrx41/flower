#pragma once

typedef struct Mutex Mutex;

Mutex*  Mutex_Create            (const strcrc* local_name, uint8 max_lock_count);
void    Mutex_Destroy           (Mutex* mutex);
void    Mutex_Lock              (Mutex* mutex, float time_out_seconds);
bool    Mutex_TryLock           (Mutex* mutex, float time_out_seconds);
void    Mutex_UnLock            (Mutex* mutex);

bool    Mutex_IsLock            (Mutex* mutex);
bool    Mutex_IsFullLock        (Mutex* mutex);


void    Mutex_WaitUnLock        (Mutex* mutex, float time_out_seconds);
bool    Mutex_TryWaitUnLock     (Mutex* mutex, float time_out_seconds);
