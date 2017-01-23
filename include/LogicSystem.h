#ifndef LOGICSYSTEM_H
#define LOGICSYSTEM_H

#include "System.h"

class LogicSystem : public System
{
public:
    LogicSystem();
    ~LogicSystem();

    void ProcessLogicComponents();
};

#endif