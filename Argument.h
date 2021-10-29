#pragma once

typedef struct argv argv;

struct argv
{
    void* argument[4];
};

argv Arg_Make1(void* arg0);
argv Arg_Make2(void* arg0, void* arg1);
argv Arg_Make3(void* arg0, void* arg1, void* arg2);
argv Arg_Make4(void* arg0, void* arg1, void* arg2, void* arg3);

#define _ARG_MAKE(m1,m2,m3,m4,m5)   m5
#define ARG_MAKE(...)   _ARG_MAKE(__VA_ARGS__, Arg_Make3, Arg_Make3, Arg_Make2, Arg_Make1)(__VA_ARGS__)
