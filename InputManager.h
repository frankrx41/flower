#pragma once

typedef struct InputManager InputManager;
typedef enum KeyId KeyId;
typedef enum KeyActivateState KeyActivateState;

enum KeyActivateState
{
    KeyActivateState_Hold,
    KeyActivateState_Down,
    KeyActivateState_Up,
    KeyActivateState_UpOnce,
    KeyActivateState_DownOnce,
    KeyActivateState_Toggle,
};

void    InputManager_Keys_UpdateState   (InputManager* input_manager, float delta_second);
void    InputManager_Event_Send         (InputManager* input_manager, const tchar* local_name);

void    InputManager_InputActionEvent_Add   (InputManager* input_manager, KeyId key_id, KeyActivateState key_activate_state, Event event);
void    InputManager_InputActionEvent_Del   (InputManager* input_manager, Event event);
void    InputManager_InputActionEvent_Clear (InputManager* input_manager);


InputManager* InputManager_GetInstance  ();

COMPILE_TIME_REQUIRE(EVENT);
