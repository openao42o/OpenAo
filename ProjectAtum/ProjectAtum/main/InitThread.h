#pragma once

#include "AtumThread.h"

class CInitThread : public CAtumThread  
{
public:
	DWORD Run() override;
};
