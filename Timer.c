#include "CoreMini.h"

#include "MemoryManager.h"
#include "TimingManager.h"

#include "Timer.h"

struct Timer
{
    float m_start_tick;
};

Timer* Timer_Create(const tchar* local_name)
{
    Timer* timer = MemNew(local_name, Timer);
    timer->m_start_tick = TimingManager_Cpu_Seconds_Get(TimingManager_GetInstance());
    return timer;
}

void Timer_Destroy(Timer* timer)
{
    Assert(timer != NULL, "");
    MemDel(timer);
}

float Timer_Elapsed_Time_Get(Timer* timer)
{
    Assert(timer != NULL, "");
    return TimingManager_Cpu_Seconds_Get(TimingManager_GetInstance()) - timer->m_start_tick;
}
