#include "CoreMini.h"

#include "MemoryManager.h"

#include "Mutex.h"
#include "String.h"
#include "Timer.h"

struct Mutex
{
    uint8   m_lock_count;
    uint8   m_lock_count_max;
    String* m_local_name;
    uint32  m_statistics_;
};


Mutex* Mutex_Create(const tchar* local_name, uint8 max_lock_count)
{
    Mutex* mutex = MemNew(local_name, Mutex);
    MemZero(mutex);

    Assert( max_lock_count > 0, "max_lock_count must be at least 1, or mutex can not be lock!" );

    mutex->m_lock_count     = 0;
    mutex->m_lock_count_max = max_lock_count;
    mutex->m_local_name     = String_New(local_name, local_name, true);

    return mutex;
}

void Mutex_Destroy(Mutex* mutex)
{
    Assert(mutex->m_lock_count != 0, "You can not delete a locked mutex!");
    MemDel(mutex);
}

void Mutex_Lock(Mutex* mutex, float time_out_seconds)
{
    bool success = Mutex_TryLock(mutex, time_out_seconds);
    Assert(success == true, "");
}

bool Mutex_TryLock(Mutex* mutex, float time_out_seconds)
{
    bool can_lock = false;
    if( mutex->m_lock_count < mutex->m_lock_count_max )
    {
        can_lock = true;
    }
    else
    {
        Timer* timer = Timer_Create(String_CStr(mutex->m_local_name));
        for(;;)
        {
            if( Timer_Elapsed_Time_Get(timer) > time_out_seconds )
            {
                can_lock = false;
                break;
            }

            if( mutex->m_lock_count < mutex->m_lock_count_max )
            {
                can_lock = true;
                break;
            }
        }
        Timer_Destroy(timer);
    }

    if( can_lock )
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
    Assert(mutex->m_lock_count > 0, "You try to unlock a unlocked mutex!");
    mutex->m_lock_count--;
}

