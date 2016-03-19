#include "main.h"

//x-window
Display* dpy;
int scr;

//system constants
unsigned long bkpix, whpix;
Atom WM_DELETE_WINDOW;
Atom PSEUDO_WM_INIT; 
Atom PSEUDO_WM_QUIT; 
Atom PSEUDO_WM_ACTIVATE;
Atom PSEUDO_WM_DEACTIVATE;

void CommonInit()
{
	dpy = XOpenDisplay(NULL);
	if (dpy == NULL) 
	{
		fprintf(stderr, "Cannot open display\n");
		exit(1);
	}

	scr = DefaultScreen(dpy);
	
	bkpix = BlackPixel(dpy, scr);
	whpix = WhitePixel(dpy, scr);
	
	WM_DELETE_WINDOW = XInternAtom(dpy, "WM_DELETE_WINDOW", False); 
	
	PSEUDO_WM_INIT = XInternAtom(dpy, "PSEUDO_WM_INIT", False); 
	PSEUDO_WM_QUIT = XInternAtom(dpy, "PSEUDO_WM_QUIT", False); 
	PSEUDO_WM_ACTIVATE = XInternAtom(dpy, "PSEUDO_WM_ACTIVATE", False); 
	PSEUDO_WM_DEACTIVATE = XInternAtom(dpy, "PSEUDO_WM_DEACTIVATE", False); 
}

void CommonQuit()
{
	XCloseDisplay(dpy);
}


typedef struct Hints
{
    unsigned long   flags;
    unsigned long   functions;
    unsigned long   decorations;
    long            inputMode;
    unsigned long   status;
} Hints;

 
void RemoveWindowBorders(Window& win)
{
	Hints hints;
	hints.flags = 2;
	hints.decorations = 0;
	Atom property = XInternAtom(dpy,"_MOTIF_WM_HINTS",True);
	XChangeProperty(dpy,win,property,property,32,PropModeReplace,(unsigned char *)&hints,5);	
}

void CommonWindowInit(const char* name, Window& win, GC& gc)
{
	win = XCreateSimpleWindow(dpy, RootWindow(dpy, scr), 10, 10, 10, 10, 1, bkpix, whpix);
	
	RemoveWindowBorders(win);
 	
	XSelectInput(dpy, win, ExposureMask);
	XStoreName(dpy, win, name);
	
    gc = XCreateGC (dpy, win, 0, 0);
    XSetBackground (dpy, gc, whpix); 
    XSetForeground (dpy, gc, bkpix); 

	XSetWMProtocols(dpy, win, &WM_DELETE_WINDOW, 1);  
}


void InvalidateWindow(Window& win)
{
	XEvent e;
	memset(&e, 0, sizeof(e));
	e.xexpose.window = win;
	e.type = Expose;
	XSendEvent(dpy,win,false,ExposureMask,&e);
	XFlush(dpy);
}

void KeyDecode(XEvent& e, KeySym& keysym, unsigned int& state, char& ch)
{
	char buf[4] = {0};
	
	int len = XLookupString(&e.xkey, buf, sizeof buf, &keysym, NULL);
	len=len;
	
	ch='\0'; 
	if (buf[0]!='\0' && buf[1]=='\0') 
	{ 
		ch = buf[0]; 
	}
	
	state = e.xkey.state;
}

pid_t pcreate(FILE*& p2c_pipe, FILE*& c2p_pipe, const char* fname, const char** argv) 
{
    if (argv==NULL)
    {
		const char* nostr = NULL;
		const char* nostrarr[1] = { nostr };
		argv = notsrarr;
	}
	
	const int readend=0; 
	const int writeend=1;
	int p2c_fd[2]; //parent writes -> child reads 
	int c2p_fd[2]; //child writes -> parent reads 
	
    pipe(&p2c_fd);
    pipe(&c2p_fd);

    pid_t pid = fork();

    if (pid>0) 
	{
		//parent
		FILE *fdopen(int fd, const char *mode);
		return pid;
	}
	else
    {
		//child
		dup2(p2c_fd[readend], STDIN_FILENO);
		dup2(c2p_fd[writeend], STDOUT_FILENO);
		pclose(p2c_fd);
		pclose(c2p_pipe);
		execvp(fname, argv);
		Log("RunPiped ERROR in exec: fname=[%s] errno=[%d]",fname,errno);
		exit(-1); //error
		return 0;
	}
}

void Log(const char* message, const char* strpar, const int intpar, const char charpar)
{
	printf(message, strpar, intpar, charpar);
	printf("\n");
}
