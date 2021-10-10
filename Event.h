#pragma once

typedef enum Event Event;

enum Event
{
    Event_Null,

    Event_Tick,

    Event_Scene_Min,
    Event_Scene_Tick = Event_Scene_Min,
    Event_Scene_Physics_Update,
    Event_Scene_Max,

    Event_Key_Min,
    Event_Key_Any_Down = Event_Key_Min,
    Event_Key_Any_Down_Once,
    Event_Key_Any_Up_Once,
    Event_Key_Any_Hold,
    Event_Key_Any_Toggle,
    Event_Key_Max,

    Event_Actor_Min,
    Event_Actor_Action_MoveUp = Event_Actor_Min,
    Event_Actor_Action_MoveDown,
    Event_Actor_Action_MoveLeft,
    Event_Actor_Action_MoveRight,
    Event_Actor_Max,

    Event_Max
};

#define EVENT_IS_DEFINED 1
