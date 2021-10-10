#pragma once

typedef struct InputManager InputManager;
typedef enum KeyId KeyId;
typedef enum KeyState KeyState;

enum KeyState
{
    KeyState_Hold,
    KeyState_Down,
    KeyState_Up,
    KeyState_UpOnce,
    KeyState_DownOnce,
    KeyState_Toggle,
};

void    InputManager_Keys_UpdateState   (InputManager* input_manager, float delta_second);
void    InputManager_Event_Send         (InputManager* input_manager);

void    InputManager_InputActionEvent_Add   (InputManager* input_manager, KeyId key_id, KeyState key_state, Event event);
void    InputManager_InputActionEvent_Del   (InputManager* input_manager, Event event);
void    InputManager_InputActionEvent_Clear (InputManager* input_manager);


InputManager* InputManager_GetInstance  ();

COMPILE_TIME_REQUIRE(EVENT);
