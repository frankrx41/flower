#pragma once


typedef struct Argument Argument;

Argument*   Argument_New    (const strcrc* local_name, uint32 count, ...);
void        Argument_Del    (Argument* argument);
tdata       Argument_Get    (const Argument* argument, uint32 index);


#define ARG_New1(local_name, type0, data0)   Argument_New(local_name, 1, tData(type0, data0))
#define ARG_New2(local_name, type0, data0, type1, data1)   Argument_New(local_name, 2, tData(type0, data0), tData(type1, data1))

