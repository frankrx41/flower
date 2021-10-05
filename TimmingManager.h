#pragma once

typedef struct TimmingManager  TimmingManager;


void    TimmingManager_SetFrameRate             (TimmingManager* timming_manager, float fps);
float   TimmingManager_GetFrameRate             (TimmingManager* timming_manager);
void    TimmingManager_SetFrameRateLimit        (TimmingManager* timming_manager, bool is_limit);
bool    TimmingManager_IslimitFrameRate         (TimmingManager* timming_manager);
float   TimmingManager_GetPrevFrameDeltaSeconds (TimmingManager* timming_manager);

void    TimmingManager_TrimSpeed                (TimmingManager* timming_manager);

TimmingManager* TimmingManager_GetInstance      ();
