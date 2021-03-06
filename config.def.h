/* See LICENSE file for copyright and license details. */
#include <X11/XF86keysym.h>
/* appearance */
static const unsigned int borderpx = 3;		  /* border pixel of windows */
static const unsigned int snap = 32;		  /* snap pixel */
static const unsigned int systraypinning = 0; /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayspacing = 2; /* systray spacing */
static const int systraypinningfailfirst = 1; /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray = 1;			  /* 0 means no systray */
static const int showbar = 1;				  /* 0 means no bar */
static const int topbar = 1;				  /* 0 means bottom bar */
static const char *fonts[] = {"Cantarell-Regular:size=12"};
static const char dmenufont[] = "Cantarell-Regular:size=12";
static const char col_gray1[] = "#1B2224"; /* top bar d */
static const char col_gray2[] = "#A4ABAA";/*unfocused fonts d */
static const char col_gray3[] = "#686868";/*unfocused border d */
static const char col_gray4[] = "#FFFFFF";/*focused fonts d */
static const char col_gray5[] = "#2EB398";/*focused windows d */
static const char col_cyan[] = "#2EB398";/*focused dmenu or topbar d */
static const char *colors[][3] = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray2, col_gray1, col_gray3 },
	[SchemeSel]  = { col_gray4, col_cyan,  col_gray5  },
	[SchemeHid]  = { col_cyan,  col_gray1, col_cyan  },
};

/* tagging */
static const char *tags[] = {"1", "2", "3", "4", "5", "6", "7", "8", "9"};
static const char *tagsalt[] = { "a", "b", "3", "4", "5", "6", "7", "8", "9" };

static const char *upvol[] = {"/usr/bin/pactl", "set-sink-volume", "0", "+5%", NULL};
static const char *downvol[] = {"/usr/bin/pactl", "set-sink-volume", "0", "-5%", NULL};
static const char *mutevol[] = {"/usr/bin/pactl", "set-sink-mute", "0", "toggle", NULL};

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{"Gimp", NULL, NULL, 0, 1, -1},
};

/* layout(s) */
static const float mfact = 0.55;  /* factor of master area size [0.05..0.95] */
static const int nmaster = 1;	 /* number of clients in master area */
static const int resizehints = 1; /* 1 means respect size hints in tiled resizals */

#include "tcl.c"
#include "layouts.c"

#include "gridmode.c"
static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "+",      tile },    /* first entry is default */
	{ "#",      grid },
	{ "-",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
	{ "|||",      tcl },
	{ "H[]",      deck },
	{ NULL,       NULL },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY, TAG)                                          \
	{MODKEY, KEY, comboview, {.ui = 1 << TAG}},                         \
		{MODKEY | ControlMask, KEY, toggleview, {.ui = 1 << TAG}}, \
		{MODKEY | ShiftMask, KEY, combotag, {.ui = 1 << TAG}},          \
		{MODKEY | ControlMask | ShiftMask, KEY, toggletag, {.ui = 1 << TAG}},


#define SHCMD(cmd)                                           \
	{                                                        \
		.v = (const char *[]) { "/bin/sh", "-c", cmd, NULL } \
	}

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = {"dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, "-h", "31", NULL};
static const char *dmenustcmd[] = {"dmenu_run_st", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL};
static const char *termcmd[] = {"st", NULL};
static const char *papercmd[] = {"paperapps", NULL};
static const char *nautiluscmd[] = {"nautilus", NULL};
static const char *slockcmd[] = {"sslock", NULL};
static const char *slockmcmd[] = {"sslock", "dmenu", NULL};
static const char *dswitchcmd[] = {"dswitch", NULL};
static const char *sucklessshutdowncmd[] = {"sucklessshutdown", NULL};
static const char *notifycmd[] = {"pbnotify", NULL};
static const char  *rangercmd[] = { "st", "-e", "sh", "-c", "ranger", NULL };
static const char *slingscold[] = { "panther_launcher", NULL};
static const char  *clickcmd[] = { "autoclicker", NULL };

#include "push.c"

static Key keys[] = {
	/* modifier                     key        function        argument */
	{MODKEY, XK_r, spawn, {.v = rangercmd } },
	{MODKEY, XK_n, spawn, {.v = nautiluscmd } },
	{MODKEY, XK_q, spawn, {.v = sucklessshutdowncmd } },
	{MODKEY, XK_y, spawn, {.v = slingscold} },
	{MODKEY, XK_a, spawn, {.v = papercmd} },
	{MODKEY, XK_w, spawn, {.v = clickcmd} },
	{MODKEY, XK_g, spawn, {.v = notifycmd} },
	{MODKEY, XK_space, spawn, {.v = dmenucmd}},
	{MODKEY, XK_minus, spawn, {.v = dmenustcmd}},
	{MODKEY, XK_x, spawn, {.v = dswitchcmd}},
	{Mod1Mask, XK_Tab, spawn, {.v = dswitchcmd}},
	{MODKEY | ControlMask, XK_l, spawn, {.v = slockcmd}},
	{MODKEY | Mod1Mask, XK_l, spawn, {.v = slockmcmd}},
	{MODKEY, XK_Return, spawn, {.v = termcmd}},
	{MODKEY, XK_b, togglebar, {0}},
	{MODKEY, XK_j, focusstack, {.i = +1}},
	{MODKEY, XK_Down, focusstack, {.i = +1}},
	{MODKEY, XK_k, focusstack, {.i = -1}},
	{MODKEY, XK_Up, focusstack, {.i = -1}},
	{MODKEY|ControlMask, XK_j, pushdown, {0} },
	{MODKEY|ControlMask, XK_k, pushup, {0} },
	{MODKEY, XK_s, togglealttag, {0} },
	{MODKEY|ShiftMask, XK_f, togglefakefullscreen, {0} },
	{MODKEY|ShiftMask, XK_w, warpfocus, {0} },
	{MODKEY|ShiftMask, XK_s, toggleshowtags, {0} },
	{MODKEY, XK_i, incnmaster, {.i = +1}},
	{MODKEY, XK_d, incnmaster, {.i = -1}},
	{MODKEY, XK_h, setmfact, {.f = -0.05}},
	{MODKEY, XK_l, setmfact, {.f = +0.05}},
	{MODKEY | ShiftMask, XK_Return, zoom, {0}},
	{MODKEY, XK_Tab, view, {0}},
	{MODKEY | ShiftMask, XK_c, killclient, {0}},
	{MODKEY, XK_t, setlayout, {.v = &layouts[0]}},
	{MODKEY, XK_f, setlayout, {.v = &layouts[2]}},
	{MODKEY, XK_m, setlayout, {.v = &layouts[3]}},
	{MODKEY, XK_c, setlayout, {.v = &layouts[1]}},
	{ MODKEY,                       XK_Left,   viewtoleft,     {0} },
	{ MODKEY,                       XK_Right,  viewtoright,    {0} },
	{ MODKEY|ShiftMask,             XK_Left,   tagtoleft,      {0} },
	{ MODKEY|ShiftMask,             XK_Right,  tagtoright,     {0} },

	{ MODKEY|ShiftMask,				XK_j,  	moveresize,	{.i = 0}},
	{ MODKEY|ShiftMask,				XK_k,  	moveresize,	{.i = 1}},
	{ MODKEY|ShiftMask,				XK_l,  	moveresize,	{.i = 2}},
	{ MODKEY|ShiftMask,				XK_h,  	moveresize,	{.i = 3}},
	
	{ MODKEY|ControlMask,		XK_comma,  cyclelayout,    {.i = -1 } },
	{ MODKEY|ControlMask,           XK_period, cyclelayout,    {.i = +1 } },
	{MODKEY, XK_p, setlayout, {0}},
	{MODKEY | ShiftMask, XK_space, togglefloating, {0}},
	{MODKEY, XK_0, view, {.ui = ~0}},
	{MODKEY | ShiftMask, XK_0, tag, {.ui = ~0}},
	{MODKEY, XK_comma, focusmon, {.i = -1}},
	{MODKEY, XK_period, focusmon, {.i = +1}},
	{MODKEY | ShiftMask, XK_comma, tagmon, {.i = -1}},
	{MODKEY | ShiftMask, XK_period, tagmon, {.i = +1}},
	TAGKEYS(XK_1, 0)
	TAGKEYS(XK_2, 1)
	TAGKEYS(XK_3, 2)
	TAGKEYS(XK_4, 3)
	TAGKEYS(XK_5, 4)
	TAGKEYS(XK_6, 5)
	TAGKEYS(XK_7, 6)
	TAGKEYS(XK_8, 7)
	TAGKEYS(XK_9, 8){MODKEY | ShiftMask, XK_q, quit, {0}},
	{0, XF86XK_AudioLowerVolume, spawn, {.v = downvol}},
	{0, XF86XK_AudioMute, spawn, {.v = mutevol}},
	{0, XF86XK_AudioRaiseVolume, spawn, {.v = upvol}},
	{ MODKEY, XK_o, winview, {0} },

};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button1,        togglewin,      {0} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};
