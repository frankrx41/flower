#include "CoreMini.h"

#include "Argument.h"

argv Arg_Make1(void* arg0)
{
    argv argument;
    argument.argument[0] = arg0;
    return argument;
}

argv Arg_Make2(void* arg0, void* arg1)
{
    argv argument;
    argument.argument[0] = arg0;
    argument.argument[1] = arg1;
    return argument;
}

argv Arg_Make3(void* arg0, void* arg1, void* arg2)
{
    argv argument;
    argument.argument[0] = arg0;
    argument.argument[1] = arg1;
    argument.argument[2] = arg2;
    return argument;
}

argv Arg_Make4(void* arg0, void* arg1, void* arg2, void* arg3)
{
    argv argument;
    argument.argument[0] = arg0;
    argument.argument[1] = arg1;
    argument.argument[2] = arg2;
    argument.argument[3] = arg3;
    return argument;
}
