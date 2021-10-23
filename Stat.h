#pragma once

typedef struct statcycle statcycle;
typedef struct statcount statcount;
typedef struct strcrc strcrc;
typedef union tdata tdata;

#define STAT_CYCLE_FRAME_MAX  ( 60 )

struct statcycle
{
    bool    m_is_initialized;
    strcrc  m_local_name;

    tdata   m_data_frame[STAT_CYCLE_FRAME_MAX];
    uint32  m_index;

    tdata   m_data_max;
    tdata   m_data_min;
    tdata   m_data_ave;

    uint32  m_frame_count;

    uint32  m_sleep_frame_const;
    uint32  m_sleep_frame_count;
};

struct statcount
{
    bool    m_is_initialized;
    strcrc  m_local_name;

    tdata   m_data_value;
    tdata   m_data_ave;
    tdata   m_data_max;
    tdata   m_data_min;

    uint32  m_frame_count;
};



void    StatCycle_Int32_Add (statcycle* stat_cycle, int32 data);
void    StatCycle_Float_Add (statcycle* stat_cycle, float data);


tdata   StatCycle_Data_Get  (const statcycle* stat_cycle);
void    StatCycle_Reset     (statcycle* stat_cycle);

void    StatCount_Int32_Inc (statcount* stat_count, int32 data);
void    StatCount_Int32_Set (statcount* stat_count, int32 data);

