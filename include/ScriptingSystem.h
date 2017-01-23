#ifndef SCRIPTINGSYSTEM_H
#define SCRIPTINGSYSTEM_H

#include <stdarg.h>
#include <iostream>
#include <vector>
#include <memory>
#include "System.h"
#include "ScriptComponent.h"

class ScriptingSystem : public System
{
public:
	ScriptingSystem();
	~ScriptingSystem();

	void ProcessScripts();
};

#endif