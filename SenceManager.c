#include "CoreMini.h"

#include "MemoryManager.h"
#include "SenceManager.h"

#include "Queue.h"


struct SenceManager
{
    Queue(Sence*)*  m_sence_queue;

    Sence*          m_current_sence;
};

Sence*  Sence_Create    (const tchar* local_name);
void    Sence_Destroy   (Sence* sence);

#undef SenceManager_Sence_Create
#undef SenceManager_SetCurrentSence
#undef SenceManager_GetCurrentSence
#undef SenceManager_Sence_Destroy
#undef SenceManager_Sence_SetCurrent
#undef SenceManager_Sence_GetCurrent

////////////////////////////////////////////////////////////////////////////////
SenceManager* SenceManager_Create(const tchar* local_name)
{
    SenceManager* sence_manager     = MemNew(local_name, SenceManager);
    sence_manager->m_sence_queue    = Queue_Create(local_name, Sence*);
    sence_manager->m_current_sence  = NULL;

    return sence_manager;
}

void SenceManager_Destroy(SenceManager* sence_manager)
{
    Queue_Destroy(sence_manager->m_sence_queue, Sence_Destroy);
    MemDel(sence_manager);
}

Sence* SenceManager_Sence_Create(SenceManager* sence_manager, const tchar* local_name)
{
    Sence* sence = Sence_Create(local_name);
    Queue_Push(Sence*, local_name, sence_manager->m_sence_queue, sence);

    return sence;
}

Sence* SenceManager_Sence_Destroy(SenceManager* sence_manager, Sence* sence)
{
    Queue_RemoveFindFirst(Sence*)(sence_manager->m_sence_queue, NULL, sence);
    Sence_Destroy(sence);
    return sence;
}

void SenceManager_Sence_SetCurrent(SenceManager* sence_manager, Sence* sence)
{
    sence_manager->m_current_sence = sence;
}

Sence* SenceManager_Sence_GetCurrent(SenceManager* sence_manager)
{
    return sence_manager->m_current_sence;
}

