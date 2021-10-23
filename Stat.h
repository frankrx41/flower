#pragma once

typedef struct StatDataCycle StatDataCycle;
typedef struct StatDataCount StatDataCount;
typedef struct strcrc strcrc;
typedef union tdata tdata;

#define STAT_CYCLE_FRAME_MAX  ( 60 )

struct StatDataCycle
{
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

struct StatDataCount
{
    strcrc  m_local_name;
    tdata   m_data_value;
    tdata   m_data_ave;
    tdata   m_data_max;
    tdata   m_data_min;

    uint32  m_frame_count;
};



void    StatDataCycle_Int32_Add (StatDataCycle* stat_data_cycle, int32 data);
void    StatDataCycle_Int32_Set (StatDataCycle* stat_data_cycle, int32 data);

tdata   StatDataCycle_Data_Get  (const StatDataCycle* stat_data_cycle);


void    StatDataCount_Int32_Add (StatDataCount* stat_data_count, int32 data);
void    StatDataCount_Int32_Set (StatDataCount* stat_data_count, int32 data);

