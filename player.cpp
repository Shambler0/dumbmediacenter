#include "main.h"

void PlayerCommandVlc(const char cmd, const char* param=0);

//window
Window PlayerWin;
static Window& win = PlayerWin;
static GC gc;

//state
char PlayerStatus = 's';
char PlayerNowPlaying[FNameMax] = {0};
char PlayerLoadedMedia[FNameMax] = {0};

void PlayerCommand(const char cmd, const char* param/*=0*/)
{
	if (param == 0)
	{
		param = "";
	}
	
	//Log("PlayerCommand: param=[%s] code=[%d] cmd=[%c]   ", param, cmd, cmd);

	if (cmd == 'q') //quit
	{
		PlayerCommand('c');
	}
	
	if (cmd == 'o') //open & play (parameter=url, no parameter=last opened)
	{
		PlayerCommand('c');
		
		if (param[0]=='\0')
		{
			param = PlayerLoadedMedia;
		}
	}
	
	if (cmd == 'p') //play
	{
		if (PlayerNowPlaying[0]==0) { return; }
		PlayerStatus = 'p';
	}
	
	if (cmd == 'u') //pause/unpause
	{
		if (PlayerStatus=='p') { PlayerStatus = 'u'; }
		else if (PlayerStatus=='u') { PlayerStatus = 'p'; }
		else { return; }
	}
	
	if (cmd == 'd') //deactivate (stop and hide player)
	{
		PlayerCommand('c');
	}

	
	//
	//
	PlayerCommandVlc(cmd, param);
	//
	//
	

	if (cmd == 'i') //init
	{
		PlayerCommand('c');
	}

	if (cmd == 'p') //play
	{
		PlayerCommand('m');
	}

	if (cmd == 'a') //activate (show player)
	{
	}
	
	if (cmd == 'o') //open & play
	{
		safestrcpy(PlayerNowPlaying, param);
		safestrcpy(PlayerLoadedMedia, PlayerNowPlaying);
		PlayerCommand('p');
		DispatcherSaveState();
	}
	
	if (cmd == 'c') //close & clear
	{
		safestrclr(PlayerNowPlaying);
		PlayerStatus = 'c';
		DispatcherSaveState();
	}

	if (cmd == '+') //volume up
	{
	}
	
	if (cmd == '-') //volume down
	{
	}
	
	if (cmd == 'v') //prev
	{
		PlayerCommand('p');
	}
	
	if (cmd == 'n') //next
	{
		PlayerCommand('p');
	}
	
	if (cmd == 'k') //seek to...
	{
		//PlayerCommand('p');
	}

	if (cmd == 'r') //rewind
	{
	}
	
	if (cmd == 'f') //fastforward
	{
	}

	if (cmd == 'm') //normal speed
	{
	}

	if (cmd == 'w') //windowed (for debugging)
	{
	}	
}

void PlayerEvt(XEvent& e)
{	
	if (e.type == ClientMessage)
	{
		unsigned int msg = (unsigned int)e.xclient.data.l[0];
		if (msg == PSEUDO_WM_INIT) 
		{
			//init
			CommonWindowInit("Mediacenter - player control", win, gc);
			PlayerCommand('i');
			return;
		}
		
		if (msg == PSEUDO_WM_ACTIVATE)  
		{ 
			PlayerCommand('a');
			return; 
		}
		
		if (msg == PSEUDO_WM_DEACTIVATE)  
		{ 
			//PlayerCommand('d'); //other windows need to hide player
			return; 
		}

		if (msg == PSEUDO_WM_QUIT) 
		{ 
			PlayerCommand('q');
			XDestroyWindow(dpy, win); 
			return; 
		}
	}
	
	if (e.type == Expose && e.xexpose.count == 0) {}


	if (e.type == KeyPress)
	{
		KeySym keysym; unsigned int state; char ch; KeyDecode(e, keysym, state, ch);
		
		if (keysym==XK_o) { PlayerCommand('o',"/home/s0/_/prj/src/linuxc/mediacenter/test.mpg"); }
		if (keysym==XK_s) { PlayerCommand('s'); }
		if (keysym==XK_p) { PlayerCommand('p'); }
		if (keysym==XK_space) { PlayerCommand('u'); }
		if (keysym==XK_Insert) { PlayerCommand('r'); }
		if (keysym==XK_Delete) { PlayerCommand('f'); }
		
		if (ch>='0' && ch<='9') { DispatcherActivateFrame( ChanWin ); }
	}
	
}
