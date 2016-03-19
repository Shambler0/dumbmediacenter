#include "main.h"

int chan=0;

Window ChanWin;
static Window& win = ChanWin;
static GC gc;

void ChanEvt(XEvent& e)
{	
	if (e.type == ClientMessage)
	{
		unsigned int msg = (unsigned int)e.xclient.data.l[0];
		if (msg == PSEUDO_WM_INIT) 
		{
			//init
			CommonWindowInit("Mediacenter - channel selector", win, gc);
			XMoveResizeWindow(dpy, win, 20, 20, 800, 200);

			const char * fontname = "-*-helvetica-*-r-*-*-200-*-*-*-*-*-*-*";
			XFontStruct* font = XLoadQueryFont(dpy, fontname);
			XSetFont (dpy, gc, font->fid);
			return;
		}
		
		if (msg == PSEUDO_WM_ACTIVATE)  
		{ 
			XMapWindow(dpy, win); 
			XRaiseWindow ( dpy, win );
			return; 
		}
		
		if (msg == PSEUDO_WM_DEACTIVATE)  
		{ 
			XUnmapWindow(dpy, win); 
			return; 
		}

		if (msg == PSEUDO_WM_QUIT) 
		{ 
			XDestroyWindow(dpy, win); 
			return; 
		}
	}
	
	if (e.type == Expose && e.xexpose.count == 0) 
	{
		XClearWindow(dpy, win);
		
		char outbuf[20] = {0};
		sprintf(outbuf, "K: %d_", chan);
		if (chan==0) { strcpy(outbuf,"K: _"); }
		XDrawString(dpy, win, gc, 10, 160, outbuf, strlen(outbuf));
	}

	if (e.type == KeyPress)
	{
		KeySym keysym; unsigned int state; char ch; KeyDecode(e, keysym, state, ch);
		
		if (ch>='0' && ch<='9')
		{
			int digit = ch - '0';
			chan *= 10;
			chan += digit;
			if (chan>9999) {chan=0;}
		}
		
		if (keysym == XK_BackSpace) 
		{ 
			chan /= 10; 
		}
			
		InvalidateWindow(win);
	}
	
}
