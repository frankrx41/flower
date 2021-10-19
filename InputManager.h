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

void    InputManager_KeysState_Update           (InputManager* input_manager, float delta_second);
void    InputManager_ControlEvent_Distribute    (InputManager* input_manager);

void    InputManager_Input_ControlEvent_Add     (InputManager* input_manager, KeyId key_id, KeyState key_state, Event event);
void    InputManager_Input_ControlEvent_Del     (InputManager* input_manager, Event event);
void    InputManager_Input_ControlEvent_Clear   (InputManager* input_manager);


InputManager* InputManager_GetInstance  ();

COMPILE_TIME_REQUIRE(EVENT);
