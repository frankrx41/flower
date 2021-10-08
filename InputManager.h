#pragma once

typedef struct InputManager InputManager;
typedef enum KeyId KeyId;

void    InputManager_Keys_UpdateState   (InputManager* input_manager, float delta_second);
void    InputManager_Event_Send         (InputManager* input_manager, const tchar* local_name);

InputManager* InputManager_GetInstance  (const Engine* engine);
