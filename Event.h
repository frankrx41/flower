#pragma once

typedef enum Event EventId;

enum Event
{
    Event_Null,

    Event_Key_Min,
    Event_Key_Any_Down          = Event_Key_Min,
    Event_Key_Any_Down_Once,
    Event_Key_Any_Up_Once,
    Event_Key_Any_Hold,
    Event_Key_Any_Toggle,
    Event_Key_Max,

    Event_Scene_Min,
    Event_Scene_Tick            = Event_Scene_Min,
    Event_Scene_Idle,

    Event_Scene_Update_Anime,
    Event_Scene_Update_Physics,

    Event_Scene_Max,


    Event_Control_Min,
    Event_Control_Any           = Event_Control_Min,
    Event_Control_MoveUp,
    Event_Control_MoveDown,
    Event_Control_MoveLeft,
    Event_Control_MoveRight,

    Event_Control_LookUp,
    Event_Control_LookDown,
    Event_Control_LookLeft,
    Event_Control_LookRight,

    Event_Control_Main,
    Event_Control_Cancel,
    Event_Control_OpenBackpack,
    Event_Control_Fire,
    Event_Control_Cover,
    Event_Control_Pause,
    Event_Control_Start,

    Event_Control_Max,

    Event_Max
};

#define EVENT_IS_DEFINED 1
