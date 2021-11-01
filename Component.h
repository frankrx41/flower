#pragma once


typedef enum ComponentEnum Component;

enum ComponentEnum
{
    Component_Min,
    Component_Render = Component_Min,
    Component_Control,
    Component_Physics,
    Component_Storage,
    Component_Max,
};

