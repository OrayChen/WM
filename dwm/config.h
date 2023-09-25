/* See LICENSE file for copyright and license details. */
#include "mpdcontrol.c"

/* appearance */
static const unsigned int borderpx  = 2;        /* border pixel of windows */
static const unsigned int gappx     = 6;        /* gaps between windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const Bool viewontag			= True;	/* Switch view on tag switch */
static const char *fonts[]          = { "FiraCode Nerd Font:style=Regular:size=18", "Symbols Nerd Font:pixelsize=18:type=2048-em" };
static const char dmenufont[]       = "FiraCode Nerd Font:style=Regular:size=18";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#cceeff";
static const char col_black[] 	    = "#000000";
static const char col_red[] 	    = "#ff0000";
static const char col_yellow[]	    = "#ffff00";
static const char col_white[] 	    = "#ffffff";
static const unsigned int baralpha = 0x5f;
static const unsigned int borderalpha = OPAQUE;
static const char *colors[][3]      = {
	/*					fg				bg				border   */
	[SchemeNorm]	= { col_gray3,		col_black,		col_gray1	},
	[SchemeSel]		= { col_white,		col_black,		col_gray1	},
	[SchemeWarn]	= { col_black,		col_yellow,		col_red		},
	[SchemeUrgent]	= { col_white,		col_red,		col_red		},
	[SchemeHov]		= { col_white,		col_black,		col_gray1	},
	[SchemeHid]		= { col_white,		col_gray1,		col_black	},
};
static const unsigned int alphas[][3]      = {
    /*               fg      bg        border*/
	[SchemeNorm] = { OPAQUE, baralpha, borderalpha },
	[SchemeSel]  = { OPAQUE, baralpha, borderalpha },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       0,            1,           -1 },
};

/* layout(s) */
static const float mfact		= 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster		= 1;    /* number of clients in master area */
static const int resizehints	= 0;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1;	/* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     	arrange function */
	{ "[T]",      	tile },    /* first entry is default */
	{ "[F]",      	NULL },    /* no layout function means floating behavior */
	{ "[M]",      	monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[]		= { "rofi", "-show", "drun", NULL };
static const char *termcmd[]		= { "st", NULL };
static const char scratchpadname[]	= "scratchpad";
static const char *scratchpadcmd[]	= { "st", "-t", scratchpadname, "-g", "100x28", NULL };
static const char *browsercmd[]		= { "google-chrome-stable", NULL };
static const char *shutdowncmd[]	= { "shutdown", "0", NULL };
static const char *rebootcmd[]		= { "reboot", NULL };
static const char *lockcmd[]		= { "slock", NULL };
static const char *musicfoxcmd[]	= { "st", "musicfox", NULL };
static const char *trayercmd[]		= { "/home/csc/.dwm/scripts/trayer.sh", NULL };
static const char *grimcmd[]		= { "/home/csc/.dwm/scripts/grim.sh", NULL };

static const Key keys[] = {
	/* modifier                     key        	function        	argument */
	{ MODKEY,                       XK_space,  	spawn,          	{ .v = dmenucmd } },
	{ MODKEY,						XK_q, 	   	spawn,          	{ .v = termcmd } },
	{ MODKEY,						XK_g,	   	spawn,				{ .v = browsercmd } },
	{ MODKEY|ShiftMask,				XK_r, 	   	spawn,				{ .v = rebootcmd } },
	{ MODKEY|ShiftMask,				XK_o,	   	spawn,				{ .v = shutdowncmd } },
	{ MODKEY,						XK_p,		togglescratch,		{ .v = scratchpadcmd } },
	{ MODKEY,                       XK_b,      	togglebar,      	{0} },
	{ MODKEY,                       XK_l,      	focusstackvis,  	{ .i = +1 } },
	{ MODKEY,                       XK_j,      	setmfact,       	{ .f = -0.05 } },
	{ MODKEY,                       XK_k,      	setmfact,       	{ .f = +0.05 } },
	{ MODKEY,						XK_o,      	killclient,     	{0} },
	{ MODKEY|ShiftMask,				XK_l,		spawn,				{ .v = lockcmd } },
	{ MODKEY,						XK_s,      	togglefloating, 	{0} },
	{ MODKEY,						XK_f,		fullscreen,			{0} },
	{ MODKEY,						XK_u,		hide,				{0} },
	{ MODKEY,						XK_i,		showall,			{0} },
	{ MODKEY,						XK_t,		spawn,				{ .v = trayercmd } },
	{ MODKEY,						XK_n,		spawn,				{ .v = grimcmd } },
	TAGKEYS(                        XK_1,      	                	0)
	TAGKEYS(                        XK_2,      	                	1)
	TAGKEYS(                        XK_3,      	                	2)
	TAGKEYS(                        XK_4,      	                	3)
	TAGKEYS(                        XK_5,      	                	4)
	TAGKEYS(                        XK_6,       	                5)
	TAGKEYS(                        XK_7,       	                6)
	TAGKEYS(                        XK_8,       	                7)
	TAGKEYS(                        XK_9,       	                8)
	{ MODKEY,						XK_comma,	mpdchange,			{ .i = -1} },
	{ MODKEY,						XK_period,	mpdchange,			{ .i = +1} },
	{ MODKEY,						XK_m,		spawn,				{ .v = musicfoxcmd } },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
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

static const char *ipcsockpath = "/tmp/dwm.sock";
static IPCCommand ipccommands[] = {
  IPCCOMMAND(  view,                1,      {ARG_TYPE_UINT}   ),
  IPCCOMMAND(  toggleview,          1,      {ARG_TYPE_UINT}   ),
  IPCCOMMAND(  tag,                 1,      {ARG_TYPE_UINT}   ),
  IPCCOMMAND(  toggletag,           1,      {ARG_TYPE_UINT}   ),
  IPCCOMMAND(  tagmon,              1,      {ARG_TYPE_UINT}   ),
  IPCCOMMAND(  focusmon,            1,      {ARG_TYPE_SINT}   ),
  IPCCOMMAND(  focusstack,          1,      {ARG_TYPE_SINT}   ),
  IPCCOMMAND(  zoom,                1,      {ARG_TYPE_NONE}   ),
  IPCCOMMAND(  incnmaster,          1,      {ARG_TYPE_SINT}   ),
  IPCCOMMAND(  killclient,          1,      {ARG_TYPE_SINT}   ),
  IPCCOMMAND(  togglefloating,      1,      {ARG_TYPE_NONE}   ),
  IPCCOMMAND(  setmfact,            1,      {ARG_TYPE_FLOAT}  ),
  IPCCOMMAND(  setlayoutsafe,       1,      {ARG_TYPE_PTR}    ),
  IPCCOMMAND(  quit,                1,      {ARG_TYPE_NONE}   )
};

