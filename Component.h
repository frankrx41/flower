#pragma once


typedef enum Component Component;

enum Component
{
    Component_Min,
    Component_Render = Component_Min,
    Component_Action,
    Component_Location,
    Component_Storage,
    Component_Max,
};

