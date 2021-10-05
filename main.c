#include "CoreMini.h"

#include "Engine.h"
#include "Render.h"
#include "Sence.h"
#include "Actor.h"
#include "Queue.h"
#include "Component.h"


int32 main()
{
    String_Test2();
    // Storage_Test0();
    // Queue_Test0();
}

void Actor_Test0()
{
    RenderManager_Initialize();

    Sence* sence = Sence_Create("sence");
    Actor* actor = Sence_Actor_Create("actor", sence);

    Actor_Component_Add(actor, Render);
    Actor_RenderComponent_AddRenderData2D(actor, 0, 10, "hello world" );

    RenderManager_RenderSenceActor(sence);

    RenderManager_RenderToScreen();

    Actor_Component_Del(actor, Render);
}

void Engine_Test0()
{
    // Engine_Initialize();
    //
    // Engine_MainLoop();
    //
    // Engine_SetSence();

}