#include "main.h"

FILE* outpipe = 0;

static const char* PlaceholderVideo = "./placeholder.avi";

void act(const char* action, const char* param=0)
{
	Log("Player act: action=[%s]  ", action);
	if (param!=0) { Log("Player act:         param=[%s]  ", param); }
	if (outpipe == 0) { Log("Player act: OutPipe is NULL!"); }
	
	fprintf(outpipe, "%s%s%s\n", action, (param==0) ? "" : " ", (param==0) ? "" : param);
	fflush(outpipe);
	
	//sleep(1);
}

void PlayerCommandVlc(const char cmd, const char* param=0);

void PlayerCommandVlc(const char cmd, const char* param)
{
	if (param == 0)
	{
		param = "";
	}
	
	
	Log("PlayerCommandVlc: param=[%s] code=[%d] cmd=[%c]   ", param, cmd, cmd);
	if (cmd != 'i' && outpipe == 0) { Log("PlayerCommandVlc: OutPipe is NULL!"); }
	
	if (cmd == '*') //arbitary command
	{
		act(param);
	}
	
	if (cmd == 'i') //init
	{
		outpipe = popen("cvlc -I rc", "w");
		//outpipe = popen("cvlc -I rc >/dev/pts/6 2>/dev/pts/6", "w");
		usleep(300000);
		Log("");
		act("loop","on");
	}
	
	if (cmd == 'q') //quit
	{
		PlayerCommandVlc('d');
		act("shutdown");
		fclose(outpipe);
		outpipe=0;
	}
	
	if (cmd == 'z') //no-signal
	{
		PlayerCommandVlc('o',PlaceholderVideo);
	}
	
	if (cmd == 'a') //activate (show player)
	{
		//PlayerCommandVlc('z');
	}
	
	if (cmd == 'd') //deactivate (stop and hide player)
	{
		//act("f", "off");
	}
	
	if (cmd == 'o') //open
	{
		PlayerCommandVlc('c');
		act("add", param);
		usleep(300000);
		act("f", "on");
	}
	
	if (cmd == 'c') //close & clear
	{
		act("stop");
		act("clear");
	}
	
	if (cmd == 's') //stop
	{
		//PlayerCommandVlc('c');
		PlayerCommandVlc('z');
	}
	
	if (cmd == 'p') //play
	{
		act("normal");
		act("play");
	}

	if (cmd == 'u') //pause/unpause
	{
		act("pause");
	}
	
	if (cmd == '+') //volume up
	{
		act("volup","8");
	}
	
	if (cmd == '-') //volume down
	{
		act("voldown","8");
	}
	
	if (cmd == 'v') //prev
	{
		act("prev");
	}
	
	if (cmd == 'n') //next
	{
		act("next");
	}
	
	if (cmd == 'k') //seek to...
	{
		act("seek",param);
	}

	if (cmd == 'r') //rewind
	{
		act("rewind");
	}
	
	if (cmd == 'f') //fastforward
	{
		act("fastforward");
	}

	if (cmd == 'm') //normal speed
	{
		act("normal");
	}

	if (cmd == 'w') //windowed (for debugging)
	{
		act("f","off");
		act("zoom","0.1");
	}

}
