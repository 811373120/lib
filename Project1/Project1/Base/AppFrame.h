#pragma once

#include <time.h>
class AppFrame
{
public:
	AppFrame();
	virtual ~AppFrame();

	virtual int initApp();
	virtual int readCfg();

	virtual void run();

	virtual bool ReloadData() { return true; }

	virtual appFinish() {}



private:

	char m_appName[100];
	struct timeval m_starttime;
};

