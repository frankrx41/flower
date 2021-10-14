#include "CoreMini.h"

#include "Engine.h"

void Engine_Debug_UnitTesting0();
void Engine_Debug_UnitTesting1();

int32 main()
{
    Engine_Initialize();

    // Engine_Debug_UnitTesting0();
    Engine_Debug_UnitTesting1();

    Engine_UnInitialize();
}
