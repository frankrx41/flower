#include "CoreMini.h"

#include "MemoryManager.h"

#include "Mutex.h"
#include "String.h"
#include "Thread.h"
#include "Timer.h"

struct Mutex
{
    uint8   m_lock_count;
    uint8   m_lock_count_max;
    strcrc  m_local_name;
    uint32  m_statistics_lock_time_out_count;
    uint32  m_statistics_lock_wait_and_lock_count;
    uint32  m_statistics_immediate_lock_count;
};


Mutex* Mutex_Create(const strcrc* local_name, uint8 max_lock_count)
{
    Mutex* mutex = MemNew(local_name, Mutex);
    MemZero(mutex);

    Assert( max_lock_count > 0, "max_lock_count must be at least 1, or mutex can not be lock!" );

    mutex->m_lock_count     = 0;
    mutex->m_lock_count_max = max_lock_count;
    StrCrc_Copy(local_name, &mutex->m_local_name);

    return mutex;
}

void Mutex_Destroy(Mutex* mutex)
{
    Assert(mutex->m_lock_count == 0, "You can not delete a locked mutex!");
    MemDel(mutex);
}

void Mutex_Lock(Mutex* mutex, float time_out_seconds)
{
    bool is_lock = Mutex_TryLock(mutex, time_out_seconds);
    Assert(is_lock == true, "Lock time out!");
}

bool Mutex_TryLock(Mutex* mutex, float time_out_seconds)
{
    bool is_can_lock = false;
    if( mutex->m_lock_count < mutex->m_lock_count_max )
    {
        is_can_lock = true;
        mutex->m_statistics_immediate_lock_count++;
    }
    else if( time_out_seconds > 0 )
    {
        Timer* timer = Timer_Create(&mutex->m_local_name);
        for(;;)
        {
            if( Timer_Elapsed_Time_Get(timer) > time_out_seconds )
            {
                is_can_lock = false;
                mutex->m_statistics_lock_time_out_count++;
                break;
            }

            if( mutex->m_lock_count < mutex->m_lock_count_max )
            {
                is_can_lock = true;
                mutex->m_statistics_lock_wait_and_lock_count++;
                break;
            }
            Thread_This_SleepTick();
        }
        Timer_Destroy(timer);
    }

    if( is_can_lock )
    {
        mutex->m_lock_count++;
        return true;
    }
    else
    {
        return false;
    }

}

void Mutex_UnLock(Mutex* mutex)
{
    Assert(Mutex_IsLock(mutex), "You try to unlock a unlocked mutex!");
    mutex->m_lock_count--;
}

bool Mutex_IsLock(Mutex* mutex)
{
    return mutex->m_lock_count > 0;
}

bool Mutex_IsFullLock(Mutex* mutex)
{
    return mutex->m_lock_count == mutex->m_lock_count_max;
}

void Mutex_WaitUnLock(Mutex* mutex, float time_out_seconds)
{
    bool is_unlock = Mutex_TryWaitUnLock(mutex, time_out_seconds);
    Assert(is_unlock == true, "");
}

bool Mutex_TryWaitUnLock(Mutex* mutex, float time_out_seconds)
{
    bool is_unlock = false;
    if( !Mutex_IsLock(mutex) )
    {
        is_unlock = true;
    }
    else if( time_out_seconds > 0 )
    {
        Timer* timer = Timer_Create(&mutex->m_local_name);
        for(;;)
        {
            if( Timer_Elapsed_Time_Get(timer) > time_out_seconds )
            {
                is_unlock = false;
                break;
            }

            if( !Mutex_IsLock(mutex) )
            {
                is_unlock = true;
                break;
            }
            Thread_This_SleepTick();
        }
        Timer_Destroy(timer);
    }
    return is_unlock;
}
