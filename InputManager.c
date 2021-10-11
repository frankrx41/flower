#include "CoreMini.h"

#include "KeyId.h"
#include "Event.h"

#include "MemoryManager.h"
#include "InputManager.h"
#include "EventManager.h"

#include "Queue.h"
#include "String.h"


typedef struct KeyInfo KeyInfo;
typedef struct InputActionEvent InputActionEvent;

struct KeyInfo
{
    KeyId   m_key_id;
    bool    m_is_down           :1;
    bool    m_is_hold           :1;
    bool    m_is_toggle         :1;
    bool    m_is_down_before    :1;
    bool    m_is_down_once      :1; // Just send once
    bool    m_is_up_once        :1; // Just send once
    float   m_down_seconds;
};

struct InputActionEvent
{
    KeyId               m_key_id;
    KeyState            m_key_state;
    Event               m_event;
};

static InputActionEvent* InputActionEvent_Create(const tchar* local_name, KeyId key_id, KeyState key_state, Event event)
{
    InputActionEvent* input_action_event = MemNew(local_name, InputActionEvent);
    input_action_event->m_key_id    = key_id;
    input_action_event->m_key_state = key_state;
    input_action_event->m_event     = event;

    return input_action_event;
}

static void InputActionEvent_Destroy(InputActionEvent* input_action_event)
{
    MemDel(input_action_event);
}

////////////////////////////////////////////////////////////////////////////////
struct InputManager
{
    KeyInfo     m_key_info[KeyId_Max - KeyId_Min];
    float       m_hold_need_seconds;
    tptr        m_platform_data;
    String*     m_local_name;
    Queue(InputActionEvent*)*   m_input_action_event_queue;
};

bool InputManager_Key_IsPhysicsDown_Plat(KeyId key_id);


InputManager* InputManager_Create(const tchar* local_name)
{
    InputManager* input_manager = MemNew(local_name, InputManager);
    for(uint32 i=0, max_i = ARRAY_SIZE(input_manager->m_key_info); i<max_i; i++)
    {
        KeyInfo* key_info = &input_manager->m_key_info[i];
        key_info->m_key_id          = KeyId_Min + i;
        key_info->m_is_down         = false;
        key_info->m_is_hold         = false;
        key_info->m_is_toggle       = false;
        key_info->m_is_down_once    = false;
        key_info->m_is_down_before  = false;
        key_info->m_is_up_once      = false;
        key_info->m_down_seconds    = 0.f;
    }

    input_manager->m_input_action_event_queue = Queue_Create(local_name, InputActionEvent*);
    input_manager->m_local_name         = String_New(local_name, local_name, true);
    input_manager->m_hold_need_seconds  = 1.f;
    return input_manager;
}

void InputManager_Destroy(InputManager* input_manager)
{
    Queue_Destroy(input_manager->m_input_action_event_queue, InputActionEvent_Destroy);
    String_Del(input_manager->m_local_name);

    MemDel(input_manager);
}

void InputManager_InputActionEvent_Add(InputManager* input_manager, KeyId key_id, KeyState key_state, Event event)
{
    Assert(IN_RANGE(event, Event_Actor_Action_Min, Event_Actor_Action_Max), "");
    Assert( key_state != KeyState_Up, "Not support KeyState_Up");
    InputActionEvent* input_action_event = InputActionEvent_Create(String_CStr(input_manager->m_local_name), key_id, key_state, event);

    Queue_Push(InputActionEvent*, String_CStr(input_manager->m_local_name), input_manager->m_input_action_event_queue, input_action_event);
}

static bool CallBack_Find_InputActionEvent(InputActionEvent* input_action_event, Event event)
{
    return input_action_event->m_event == event;
}

void InputManager_InputActionEvent_Del(InputManager* input_manager, Event event)
{
    Assert(IN_RANGE(event, Event_Actor_Action_Min, Event_Actor_Action_Max), "");
    InputActionEvent* input_action_event = Queue_RemoveFindFirst(InputActionEvent*)(input_manager->m_input_action_event_queue, (CB_FindData_Bool_tPtr_tPtr)CallBack_Find_InputActionEvent, (const tptr)event);
    InputActionEvent_Destroy(input_action_event);
}

void InputManager_InputActionEvent_Clear(InputManager* input_manager)
{
    Queue_Clear(input_manager->m_input_action_event_queue, InputActionEvent_Destroy);
}

void CallBack_ProcessInputActionEvent(InputActionEvent* input_action_event, InputManager* input_manager)
{
    KeyInfo* key_info = &input_manager->m_key_info[input_action_event->m_key_id];
    bool is_active = false;
    Assert( input_action_event->m_key_state != KeyState_Up, "" );
    switch(input_action_event->m_key_state)
    {
    case KeyState_Hold:     is_active = key_info->m_is_hold;        break;
    case KeyState_Down:     is_active = key_info->m_is_down;        break;
    case KeyState_Up:       is_active = false;                      break;
    case KeyState_UpOnce:   is_active = key_info->m_is_up_once;     break;
    case KeyState_DownOnce: is_active = key_info->m_is_down_once;   break;
    case KeyState_Toggle:   is_active = key_info->m_is_toggle;      break;
    }
    if( is_active )
    {
        SendEvent_Actor_Action(input_action_event->m_event);
    }
}

void InputManager_Event_Send(InputManager* input_manager)
{
    Queue_ForEach(input_manager->m_input_action_event_queue, CallBack_ProcessInputActionEvent, input_manager);
}


void InputManager_Keys_UpdateState(InputManager* input_manager, float delta_second)
{
    for(KeyId key_id=KeyId_Min; key_id<KeyId_Max; key_id++)
    {
        KeyInfo* key_info = &input_manager->m_key_info[key_id];

        key_info->m_is_down_before = key_info->m_is_down;

        if( InputManager_Key_IsPhysicsDown_Plat(key_id) )
        {
            key_info->m_is_up_once = false;

            if( key_info->m_down_seconds == 0 )
            {
                key_info->m_is_down_once   = true;
                key_info->m_is_down        = true;

                if( key_info->m_is_toggle )
                {
                    key_info->m_is_toggle = false;
                }
                else
                {
                    key_info->m_is_toggle = true;
                }
            }

            const float down_second_time = key_info->m_down_seconds;
            if( down_second_time > input_manager->m_hold_need_seconds )
            {
                key_info->m_is_hold = true;
            }
            else
            {
                key_info->m_down_seconds += delta_second;
            }
        }
        else
        {
            key_info->m_is_down        = false;
            key_info->m_is_down_once   = false;
            key_info->m_is_hold        = false;
            key_info->m_down_seconds   = 0.f;

            if( key_info->m_is_down_before )
            {
                key_info->m_is_up_once = true;
            }
        }
    }
}
