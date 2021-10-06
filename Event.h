#pragma once

typedef enum Event Event;

enum Event
{
    Event_Null,
    Event_Tick,
    Event_OnKey_Any_Down,
    Event_OnKey_Any_Up,
    Event_OnKey_Any_Hold,

    Event_OnAction_MoveUp,
    Event_OnAction_MoveDown,
    Event_OnAction_MoveLeft,
    Event_OnAction_MoveRight,

    Event_Max
};
