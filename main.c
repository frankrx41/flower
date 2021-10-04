#include "CoreMini.h"

#include "Engine.h"
#include "Render.h"
#include "Sence.h"
#include "Actor.h"
#include "Queue.h"

struct RenderContent
{
    Queue(RenderData*)* m_render_data_queue;
};

int32 main()
{
    // Engine_Initialize();
    //
    // Engine_MainLoop();
    //
    // Engine_SetSence();

    // Storage_Test0();
    // Queue_Test0();


    RenderManager_Initialize();

    Sence* sence = Sence_Create("sence");
    Actor* actor = Sence_Actor_Create("actor", sence);
    
    Actor_AddComponent(actor, RenderContent);
    
    RenderContent* render_content = Actor_Cast(actor, RenderContent);
    render_content->m_render_data_queue = Queue_Create("a", RenderData*);
    RenderContent_Render_Plat(render_content, 0, 0, "i");


    RenderManager_RenderSenceActor(sence);

    RenderManager_RenderToScreen();



}
