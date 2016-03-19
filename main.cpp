#include "main.h"

int main(int argc, char** argv)
{
	//init
	CommonInit();
	DispatcherInit();

	//loop
	DispatcherEventLoop();
	
	//quit
	DispatcherQuit();
	void CommonQuit();
	return 0;
}	
