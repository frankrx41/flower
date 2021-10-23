#include "CoreMini.h"

#include "String.h"
#include "tData.h"
#include "Stat.h"


static void StatDataCycle_UpdateIndex(statcycle* stat_cycle)
{
    stat_cycle->m_index++;
    if( stat_cycle->m_index >= STAT_CYCLE_FRAME_MAX )
    {
        stat_cycle->m_index = 0;
    }
}

static bool StatDataCycle_IsUpdateFrame(statcycle* stat_cycle)
{
    if( stat_cycle->m_sleep_frame_const == 0 )
    {
        return true;
    }

    if( stat_cycle->m_sleep_frame_count >= stat_cycle->m_sleep_frame_const )
    {
        stat_cycle->m_sleep_frame_count = 0;
        return true;
    }

    stat_cycle->m_sleep_frame_count++;
    return false;
}

static bool StatDataCycle_IsSleepFrame(statcycle* stat_cycle)
{
    return !StatDataCycle_IsUpdateFrame(stat_cycle);
}

////////////////////////////////////////////////////////////////////////////////
void StatCycle_Int32_Add(statcycle* stat_cycle, int32 data)
{
    if( StatDataCycle_IsSleepFrame(stat_cycle) )
    {
        return;
    }

    stat_cycle->m_data_frame[stat_cycle->m_index].m_int32 = data;
    stat_cycle->m_data_ave.m_int32 = (stat_cycle->m_data_ave.m_int32 * stat_cycle->m_frame_count + data) / (stat_cycle->m_frame_count+1);
    stat_cycle->m_frame_count++;

    if( !stat_cycle->m_is_initialized )
    {
        stat_cycle->m_data_min.m_int32 = data;
        stat_cycle->m_is_initialized = true;
    }

    if( data < stat_cycle->m_data_min.m_int32 )
    {
        stat_cycle->m_data_min.m_int32 = data;
    }

    if( data > stat_cycle->m_data_max.m_int32 )
    {
        stat_cycle->m_data_max.m_int32 = data;
    }

    StatDataCycle_UpdateIndex(stat_cycle);
}

void StatCycle_Float_Add(statcycle* stat_cycle, float data)
{
    if( StatDataCycle_IsSleepFrame(stat_cycle) )
    {
        return;
    }

    stat_cycle->m_data_frame[stat_cycle->m_index].m_float = data;
    stat_cycle->m_data_ave.m_float = (stat_cycle->m_data_ave.m_float * stat_cycle->m_frame_count + data) / (stat_cycle->m_frame_count+1);
    stat_cycle->m_frame_count++;

    if( !stat_cycle->m_is_initialized )
    {
        stat_cycle->m_data_min.m_float = data;
        stat_cycle->m_is_initialized = true;
    }

    if( data < stat_cycle->m_data_min.m_float )
    {
        stat_cycle->m_data_min.m_float = data;
    }

    if( data > stat_cycle->m_data_max.m_float )
    {
        stat_cycle->m_data_max.m_float = data;
    }

    StatDataCycle_UpdateIndex(stat_cycle);
}

tdata StatCycle_Data_Get(const statcycle* stat_cycle)
{
    Assert( stat_cycle->m_is_initialized == true, "" );
    return stat_cycle->m_data_frame[stat_cycle->m_index];
}

void StatCycle_Reset(statcycle* stat_cycle)
{
    stat_cycle->m_data_max.m_int32  = 0;
    stat_cycle->m_data_min.m_int32  = 0;
    stat_cycle->m_data_ave.m_int32  = 0;
    stat_cycle->m_index             = 0;
    stat_cycle->m_frame_count       = 0;
    stat_cycle->m_sleep_frame_const = 0;
    stat_cycle->m_sleep_frame_count = 0;
    stat_cycle->m_is_initialized    = false;
}

////////////////////////////////////////////////////////////////////////////////
void StatCount_Int32_Inc(statcount* stat_count, int32 data)
{
    StatCount_Int32_Set( stat_count, stat_count->m_data_value.m_int32 + data );
}

void StatCount_Int32_Set(statcount* stat_count, int32 data)
{
    stat_count->m_data_value.m_int32 = data;
    stat_count->m_data_ave.m_int32 = (stat_count->m_data_ave.m_int32 * stat_count->m_frame_count + data ) / (stat_count->m_frame_count+1);
    stat_count->m_frame_count++;
    
    if( !stat_count->m_is_initialized )
    {
        stat_count->m_data_min.m_int32 = data;
        stat_count->m_is_initialized = true;
    }
 
    if( data < stat_count->m_data_min.m_int32 )
    {
        stat_count->m_data_min.m_int32 = data;
    }

    if( data > stat_count->m_data_max.m_int32)
    {
        stat_count->m_data_max.m_int32 = data;
    }
}