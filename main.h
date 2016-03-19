#pragma once

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "macros.h"

// ----------------------------------------------------

//common
static const int FNameMax = 2100;

extern Display* dpy;
extern int scr;

extern unsigned long bkpix, whpix;
extern Atom WM_DELETE_WINDOW;
extern Atom PSEUDO_WM_INIT; 
extern Atom PSEUDO_WM_QUIT; 
extern Atom PSEUDO_WM_ACTIVATE;
extern Atom PSEUDO_WM_DEACTIVATE;

void CommonInit();
void CommonQuit();
void RemoveWindowBorders(Window& win);
void CommonWindowInit(const char* name, Window& win, GC& gc);
void InvalidateWindow(Window& win);
void KeyDecode(XEvent& e, KeySym& keysym, unsigned int& state, char& ch);

void Log(const char* message, const char* strpar="", const int intpar=0, const char charpar='^');

//dispatcher
void DispatcherInit();
void DispatcherOn();
void DispatcherOff();
void DispatcherEventLoop();
void DispatcherQuit();
void DispatcherActivateFrame(Window& win);
void DispatcherSaveState();
void DispatcherLoadState();


//player
void PlayerCommand(const char cmd, const char* param=0);
extern char PlayerLoadedMedia[FNameMax];


//windows
extern Window DispatcherWin;
extern Window ChanWin;
extern Window PlayerWin;

//events
void DispatcherEvt(XEvent& e);
void ChanEvt(XEvent& e);
void PlayerEvt(XEvent& e);
