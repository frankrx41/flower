#pragma once

typedef struct Timming  Timming;

#if PUBLIC_TIMMING
struct Timming
{
    bool    m_is_initialized;
    bool    m_is_limit_frame_rate;
    tptr    m_platform_data;

    float   m_prev_frame_delta_seconds;
    float   m_frame_rate;
};
#endif

// Timming
void    Engine_Timming_Initialize               ();
void    Engine_Timming_SetFrameRate             (float fps);
float   Engine_Timming_GetFrameRate             ();
void    Engine_Timming_SetFrameRateLimit        (bool is_limit);
bool    Engine_Timming_IslimitFrameRate         ();
float   Engine_Timming_GetPrevFrameDeltaSeconds ();

void    Engine_Timming_Initialize_Plat          ();
void    Engine_Timming_SetFrameRate_Plat        (float fps);
void    Engine_Timming_TrimSpeed_Plat           ();

Timming* Engine_Timming_GetInstance             ();
