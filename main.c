#include "CoreMini.h"

#include "Engine.h"

void Engine_Debug_UnitTesting();

int32 main()
{
    Engine* engine = Engine_GetInstance();

    Engine_Initialize(engine);

    Engine_Debug_UnitTesting(engine);

    Engine_UnInitialize(engine);
}
