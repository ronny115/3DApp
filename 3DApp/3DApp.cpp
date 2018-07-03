// 3DApp.cpp : Defines the entry point for the console application.
#include "systemclass.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	SystemClass*System;
	bool result;

	//create the system object
	System = new SystemClass;
	if(!System)
	{
		return 0;
	}

	//Initialize and rum system object
	result = System->Initialize();
	if(result)
	{
		System->Run();
	}
	
	//Shutdown and release system object
	System->Shutdown();
	delete System;
	System = 0;
	return 0;

}

