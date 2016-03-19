#include "main.h"

static const Window broadcast = 0;
static const char* DispatcherCfgFileSubPath = ".config/dumbmediacenter.txt";

Window PrevFrame = broadcast;
Window ActiveFrame = broadcast;
Window DispatcherWin;
bool DispatcherMustQuit=false; //signal to quit now
bool DispatcherGrabbedInput=false; //dispacher is on, intercepting keyboard
char DispatcherCfgFile[FNameMax] = {0};

void DispatchEventToFrames(XEvent& e, Window win)
{
	//list of all windows
	if (win==broadcast || win==DispatcherWin) { DispatcherEvt(e); }
	if (win==broadcast || win==ChanWin) { ChanEvt(e); }
	if (win==broadcast || win==PlayerWin) { PlayerEvt(e); }
}

char* GetWindowName(Window& win)
{
	char* name=0;
	if (win==broadcast)	{ return (char*) "-zero window-"; }
	XFetchName(dpy, win, &name);
	if (name==0) { return (char*) "-unknown window-"; }
	return name;
}


void DispatchEvent(XEvent& e)
{
	Window win = e.xclient.window;
	
	if (e.type == ClientMessage)
	{
		unsigned int msg = (unsigned int)e.xclient.data.l[0];
		if (msg == WM_DELETE_WINDOW) { DispatcherMustQuit=true; }
		Log("ClientMessage (IGNORED) win=[%s] msg=%d", GetWindowName(win), msg);
	}
	
	if (e.type == Expose) 
	{
		DispatchEventToFrames(e, win);
	}

	if (e.type == KeyPress)
	{
		KeySym keysym; unsigned int state; char ch; KeyDecode(e, keysym, state, ch);
		
		// global hotkeys
		if (keysym == XK_Escape) { DispatcherMustQuit=true; return; } //quit now (debug)
		if (keysym == XK_F11) { DispatcherMustQuit=true; return; } //quit now
		if (keysym == XK_F12) { PlayerCommand('w'); return; } //windowed (debug)

		// else dispatch to frame
		Log("KeyPress activeframe=[%s] keysim=[%d] char=[%c]     ", GetWindowName(ActiveFrame), keysym, ch);
		DispatchEventToFrames(e, ActiveFrame);
	}
}

XEvent GeneratePseudoEvent(Atom& msg)
{
	Log("PseudoEvent%s id=[%d]", "", (int)msg);
	XEvent e;
	memset(&e,0,sizeof(e));
	e.type = ClientMessage;
	e.xclient.data.l[0]=msg;
	return e;
}

void DispatcherActivateFrame(Window& win)
{
	if (ActiveFrame != broadcast) 
	{ 
		Log("Deactivate activeframe=[%s] msg=[%d]", GetWindowName(ActiveFrame), PSEUDO_WM_DEACTIVATE);
		XEvent e = GeneratePseudoEvent(PSEUDO_WM_DEACTIVATE);
		DispatchEventToFrames(e, ActiveFrame); 
	}
	
	PrevFrame = ActiveFrame;
	ActiveFrame = win;
	
	Log("Activate activeframe=[%s] msg=[%d]", GetWindowName(ActiveFrame), PSEUDO_WM_ACTIVATE);
	XEvent e = GeneratePseudoEvent(PSEUDO_WM_ACTIVATE);
	DispatchEventToFrames(e, win);
	
	DispatcherSaveState();
}

void DispatcherStartup()
{
	Log("Dbg PlayerWin name=[%s] id=[%d]",GetWindowName(PlayerWin),(int)PlayerWin );
	Log("Dbg ChanWin name=[%s] id=[%d]",GetWindowName(ChanWin),(int)ChanWin );
	Log("Dbg DispatcherWin name=[%s] id=[%d]",GetWindowName(DispatcherWin),(int)DispatcherWin );
	DispatcherActivateFrame(PlayerWin);
}

void DispatcherOn()
{
	if (!DispatcherGrabbedInput)
	{
		Log("Grab");
		XGrabKeyboard(dpy,DispatcherWin,False,GrabModeAsync,GrabModeAsync,CurrentTime);
		//XGrabPointer(dpy,DispatcherWin,True,0,GrabModeAsync,GrabModeAsync,DispatcherWin,0L,CurrentTime);
	}
	DispatcherGrabbedInput=true;
}

void DispatcherOff()
{
	if (DispatcherGrabbedInput)
	{
		Log("Ungrab");
		XUngrabKeyboard(dpy, CurrentTime);
		XUngrabPointer(dpy, CurrentTime);
	}
	DispatcherGrabbedInput=false;
}

void DispatcherInit()
{
	safesprintf2(DispatcherCfgFile, "%s/%s", getenv("HOME"), DispatcherCfgFileSubPath);
	DispatcherLoadState();

	ActiveFrame = DispatcherWin;
	XEvent e = GeneratePseudoEvent(PSEUDO_WM_INIT);
	DispatchEventToFrames(e, broadcast);
}

void DispatcherQuit()
{
	DispatcherOff();
	XEvent e = GeneratePseudoEvent(PSEUDO_WM_QUIT);
	DispatchEventToFrames(e, broadcast);
}

void DispatcherEventLoop()
{
	DispatcherOff();
	DispatcherOn();
	DispatcherStartup();
	
	XEvent e;
	DispatcherMustQuit=false;
	while (!DispatcherMustQuit) 
	{
		XNextEvent(dpy, &e);
		DispatchEvent(e);
	}
}

#define MP_VER_STR "mediaplayer-cfg-ver=1"

void DispatcherSaveState()
{
	Log("Saving file=[%s]", DispatcherCfgFile);
	FILE* f = fopen(DispatcherCfgFile,"w");
	if (f == NULL)
	{
		Log("Error in fopen filename=[%s] fd=[%d]", DispatcherCfgFile, (int)(int64_t) f);
	}
	else
	{
		fprintf(f, "#!/dev/null" LF);
		fprintf(f, "# DO NOT EDIT. LINE ORDER IS IMPORTANT!!!" LF);
		fprintf(f, MP_VER_STR LF);
		fprintf(f, "%s" LF, PlayerLoadedMedia);
		fclose(f);
	}
}

void DispatcherLoadState()
{
	Log("Loading file=[%s]", DispatcherCfgFile);
	char buf[FNameMax] = {0};
	FILE* f = fopen(DispatcherCfgFile,"r");
	if (f == NULL)
	{
		Log("Error in fopen filename=[%s] fd=[%d]", DispatcherCfgFile, (int)(int64_t) f);
	}
	else
	{
		safefgetsnolf(buf, f); //#!...
		safefgetsnolf(buf, f); //#...
		safefgetsnolf(buf, f); if ( strcmp(buf, MP_VER_STR) != 0 ) { return; } //wrong version
		safefgetsnolf(PlayerLoadedMedia, f);
		fclose(f);
		Log("Loaded PlayerLoadedMedia=[%s]", PlayerLoadedMedia);
	}
}


void DispatcherEvt(XEvent& e) //events for dispatcher window itself
{
	Window& win = DispatcherWin;
	static GC gc;
	
	if (e.type == ClientMessage)
	{
		unsigned int msg = (unsigned int)e.xclient.data.l[0];
		if (msg == PSEUDO_WM_INIT) 
		{
			//init
			CommonWindowInit("Mediacenter - dispatcher", win, gc);
			XSelectInput(dpy, win, ExposureMask | KeyPressMask);
			XMapWindow(dpy, win);
			return;
		}
		
		if (msg == PSEUDO_WM_ACTIVATE)  { return; }
		if (msg == PSEUDO_WM_DEACTIVATE)  { return; }
		
		if (msg == PSEUDO_WM_QUIT) { XUnmapWindow(dpy, win); XDestroyWindow(dpy, win); return; }
	}
	
	if (e.type == Expose) {}
	if (e.type == KeyPress) {}
}

