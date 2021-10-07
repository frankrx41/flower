#include "CoreMini.h"

#include "Engine.h"

void Engine_Debug_UnitTesting();

int32 main()
{
    Engine_Initialize();

    Engine_Debug_UnitTesting();

    Engine_UnInitialize();
}
