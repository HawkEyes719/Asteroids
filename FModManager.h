#pragma once

#include <fmod.hpp>
#include <fmod_errors.h>
#include <iostream>

class FModManager
{
public:
	static FMOD::System* System();

private:
	static FMOD::System *system;

};

