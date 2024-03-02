#ifndef PLUGIN_H
#define PLUGIN_H

#include <Windows.h>

class Plugin
{
public:
	virtual void Register() = 0;
	virtual void OnGameInit() = 0;
};

#endif

