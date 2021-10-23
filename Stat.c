#include "CoreMini.h"

#include "String.h"
#include "tData.h"
#include "Stat.h"

static void StatDataCycle_UpdateIndex(StatDataCycle* stat_data_cycle)
{
    stat_data_cycle->m_index++;
    if( stat_data_cycle->m_index >= STAT_CYCLE_FRAME_MAX )
    {
        stat_data_cycle = 0;
    }
}

static bool StatDataCycle_IsUpdateFrame(StatDataCycle* stat_data_cycle)
{
    if( stat_data_cycle->m_sleep_frame_const == 0 )
    {
        return true;
    }

    if( stat_data_cycle->m_sleep_frame_count >= stat_data_cycle->m_sleep_frame_const )
    {
        stat_data_cycle->m_sleep_frame_count = 0;
        return true;
    }

    stat_data_cycle->m_sleep_frame_count++;
    return false;
}

static bool StatDataCycle_IsSleepFrame(StatDataCycle* stat_data_cycle)
{
    return !StatDataCycle_IsUpdateFrame(stat_data_cycle);
}

void StatDataCycle_Int32_Add(StatDataCycle* stat_data_cycle, int32 data)
{
    StatDataCycle_Int32_Set(stat_data_cycle, StatDataCycle_Data_Get(stat_data_cycle).m_int32 + data);
}

void StatDataCycle_Int32_Set(StatDataCycle* stat_data_cycle, int32 data)
{
    if( !StatDataCycle_IsSleepFrame(stat_data_cycle) )
    {
        return;
    }

    stat_data_cycle->m_data_frame[stat_data_cycle->m_index].m_int32 = data;
    stat_data_cycle->m_data_ave.m_int32 = (stat_data_cycle->m_data_ave.m_int32 * stat_data_cycle->m_frame_count + data) / (stat_data_cycle->m_frame_count+1);
    stat_data_cycle->m_frame_count++;

    if( data < stat_data_cycle->m_data_min.m_int32 )
    {
        stat_data_cycle->m_data_min.m_int32 = data;
    }

    if( data > stat_data_cycle->m_data_max.m_int32 )
    {
        stat_data_cycle->m_data_max.m_int32 = data;
    }

    StatDataCycle_UpdateIndex(stat_data_cycle);
}

tdata StatDataCycle_Data_Get(const StatDataCycle* stat_data_cycle)
{
    return stat_data_cycle->m_data_frame[stat_data_cycle->m_index];
}

////////////////////////////////////////////////////////////////////////////////
void StatDataCount_Int32_Add(StatDataCount* stat_data_count, int32 data)
{
    StatDataCount_Int32_Set( stat_data_count, stat_data_count->m_data_value.m_int32 + data );
}

void StatDataCount_Int32_Set(StatDataCount* stat_data_count, int32 data)
{
    stat_data_count->m_data_value.m_int32 = data;
    stat_data_count->m_data_ave.m_int32 = (stat_data_count->m_data_ave.m_int32 * stat_data_count->m_frame_count + data ) / (stat_data_count->m_frame_count+1);
    stat_data_count->m_frame_count++;
    
    if( data < stat_data_count->m_data_min.m_int32 )
    {
        stat_data_count->m_data_min.m_int32 = data;
    }

    if( data > stat_data_count->m_data_max.m_int32)
    {
        stat_data_count->m_data_max.m_int32 = data;
    }
}