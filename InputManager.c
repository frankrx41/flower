#include "CoreMini.h"

#include "MemoryManager.h"
#include "InputManager.h"
#include "EventManager.h"

#include "KeyId.h"
#include "Event.h"

typedef struct KeyState KeyState;

struct KeyState
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

struct InputManager
{
    KeyState    m_key_state[KeyId_Max];
    float       m_hold_need_seconds;
};

bool InputManager_Key_IsPhysicsDown_Plat(KeyId key_id);


InputManager* InputManager_Create(const tchar* local_name)
{
    InputManager* input_manager = MemNew(local_name, InputManager);
    for(KeyId key_id=0; key_id<KeyId_Max; key_id++)
    {
        KeyState* key_state = &input_manager->m_key_state[key_id];
        key_state->m_key_id         = key_id;
        key_state->m_is_down        = false;
        key_state->m_is_hold        = false;
        key_state->m_is_toggle      = false;
        key_state->m_is_down_once   = false;
        key_state->m_is_down_before = false;
        key_state->m_is_up_once     = false;
        key_state->m_down_seconds   = 0.f;
    }
    input_manager->m_hold_need_seconds = 1.f;
    return input_manager;
}


void InputManager_Event_Send(InputManager* input_manager, const tchar* local_name)
{
    for(KeyId key_id=0; key_id<KeyId_Max; key_id++)
    {
        KeyState* key_state = &input_manager->m_key_state[key_id];

        if( key_state->m_is_hold )
        {
            EventManager_SendEvent(Event_Key_Any_Hold, local_name, key_state->m_key_id, key_state->m_down_seconds);
        }
        if( key_state->m_is_down )
        {
            EventManager_SendEvent(Event_Key_Any_Down, local_name, key_state->m_key_id);
        }
        if( key_state->m_is_down_once )
        {
            EventManager_SendEvent(Event_Key_Any_Down_Once, local_name, key_state->m_key_id);
        }
        if( key_state->m_is_toggle )
        {
            EventManager_SendEvent(Event_Key_Any_Toggle, local_name, key_state->m_key_id);
        }
        if( key_state->m_is_up_once )
        {
            EventManager_SendEvent(Event_Key_Any_Up_Once, local_name, key_state->m_key_id);
        }
    }
}


void InputManager_Keys_UpdateState(InputManager* input_manager, float delta_second)
{
    for(KeyId key_id=KeyId_Min; key_id<KeyId_Max; key_id++)
    {
        KeyState* key_state = &input_manager->m_key_state[key_id];

        key_state->m_is_down_before = key_state->m_is_down;

        if( InputManager_Key_IsPhysicsDown_Plat(key_id) )
        {
            key_state->m_is_up_once = false;

            if( key_state->m_down_seconds == 0 )
            {
                key_state->m_is_down_once   = true;
                key_state->m_is_down        = true;

                if( key_state->m_is_toggle )
                {
                    key_state->m_is_toggle = false;
                }
                else
                {
                    key_state->m_is_toggle = true;
                }
            }

            float down_second_time = key_state->m_down_seconds;
            if( down_second_time > input_manager->m_hold_need_seconds )
            {
                key_state->m_is_hold = true;
            }
            else
            {
                key_state->m_down_seconds += delta_second;
            }
        }
        else
        {
            key_state->m_is_down        = false;
            key_state->m_is_down_once   = false;
            key_state->m_is_hold        = false;
            key_state->m_down_seconds   = 0.f;

            if( key_state->m_is_down_before )
            {
                key_state->m_is_up_once = true;
            }
        }
    }
}
