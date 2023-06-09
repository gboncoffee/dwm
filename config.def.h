/* vim:set noexpandtab */
/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx = 3;        /* border pixel of windows */
static const unsigned int snap     = 32;       /* snap pixel */
static const int showbar           = 1;        /* 0 means no bar */
static const int topbar            = 1;        /* 0 means bottom bar */
static const char *fonts[]         = { "CaskaydiaCove Nerd Font:size=15" };
static const char dmenufont[]      = "CaskaydiaCove Nerd Font:size=15";
#include"colors.h"
static const char *colors[][3]     = {
	/*                      fg      bg       border */
	[SchemeNorm]        = { col_fg, col_bg,  col_bg  },
	[SchemeSel]         = { col_bg, col_sel, col_sel },
	[SchemeTabActive]   = { col_fg, col_bg,  col_bg  },
	[SchemeTabInactive] = { col_fg, col_bg,  col_bg  }
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",      NULL,       NULL,       0,            1,           -1 },
	{ "floating",  NULL,       NULL,       0,            1,           -1 },
	{ "spectacle", NULL,       NULL,       0,            1,           -1 },
};

/* layout(s) */
static const float mfact     = 0.5;  /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

/* Bartabgroups properties */
#define BARTAB_BORDERS 1       // 0 = off, 1 = on
#define BARTAB_BOTTOMBORDER 1  // 0 = off, 1 = on
#define BARTAB_TAGSINDICATOR 1 // 0 = off, 1 = on if >1 client/view tag, 2 = always on
#define BARTAB_TAGSPX 5        // # pixels for tag grid boxes
#define BARTAB_TAGSROWS 3      // # rows in tag grid (9 tags, e.g. 3x3)
static void (*bartabmonfns[])(Monitor *) = { monocle /* , customlayoutfn */ };
static void (*bartabfloatfns[])(Monitor *) = { NULL /* , customlayoutfn */ };

static const Layout layouts[] = {
	/* symbol     arrange function */
    { "[=]",      doubledeck },
	{ "[]=",      tile },
	{ "[M]",      monocle },
	{ "=<>",      NULL },
	{ NULL,       NULL },
};

/* key definitions */
#define TERM "st"
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

#include "movestack.c"

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", NULL };
static const char *termcmd[]  = { TERM, NULL };

static const char *browsercmd[] = { "firefox", NULL };
static const char *emailcmd[] = { "thunderbird", NULL };
static const char *calccmd[] = { TERM, "-c", "floating", "-e", "python", NULL };
static const char *pulsecmd[] = { TERM, "-c", "floating", "-e", "pulsemixer", NULL };
static const char *musiccmd[] = { TERM, "-c", "floating", "-e", "ncmpcpp", NULL };
static const char *fmcmd[] = { TERM, "-e", "lf", NULL };

static const char *screenshot[] = { "spectacle", NULL };
static const char *notifyclose[] = { "dunstctl", "close", NULL };

// dmenu scripts
static const char *dmenu_shutdown[] = { "dmenu_shutdown", NULL };
static const char *dmenu_web[] = { "dmenu_web", NULL };
static const char *dmenu_bluetooth[] = { "dmenu_bluetooth", NULL };
static const char *monitors[] = { "monitors", NULL };
static const char *passmenu[] = { "passmenu", NULL };

// audio
static const char *vol_up[] = { "pulsemixer", "--change-volume", "+1", NULL };
static const char *vol_down[] = { "pulsemixer", "--change-volume", "-1", NULL };
static const char *vol_mute[] = { "pulsemixer", "--toggle-mute", NULL };
static const char *mus_next[] = { "mpc", "next", NULL };
static const char *mus_prev[] = { "mpc", "prev", NULL };
static const char *mus_toggle[] = { "mpc", "toggle", NULL };

// theme
static const char *change_theme[] = { "theme_change", NULL };

static const Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_a,      spawn,          {.v = dmenucmd } },
	{ MODKEY,                       XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_n,      spawn,          {.v = browsercmd } },
	{ MODKEY,                       XK_e,      spawn,          {.v = emailcmd } },
	{ MODKEY,                       XK_c,      spawn,          {.v = calccmd } },
	{ MODKEY,                       XK_s,      spawn,          {.v = pulsecmd } },
	{ MODKEY,                       XK_m,      spawn,          {.v = musiccmd } },
	{ MODKEY,                       XK_f,      spawn,          {.v = fmcmd } },

	{ MODKEY,                       XK_p,      spawn,          {.v = screenshot } },
	{ MODKEY|ControlMask,           XK_c,      spawn,          {.v = notifyclose } },

	{ MODKEY,                       XK_q,      spawn,          {.v = dmenu_shutdown } },
	{ MODKEY,                       XK_b,      spawn,          {.v = dmenu_web } },
	{ MODKEY|ControlMask,           XK_b,      spawn,          {.v = dmenu_bluetooth } },
	{ MODKEY|ControlMask,           XK_d,      spawn,          {.v = monitors } },
	{ MODKEY|ControlMask,           XK_p,      spawn,          {.v = passmenu } },

	{ Mod1Mask|ControlMask,         XK_k,      spawn,          {.v = vol_up } },
	{ Mod1Mask|ControlMask,         XK_j,      spawn,          {.v = vol_down } },
	{ Mod1Mask|ControlMask,         XK_m,      spawn,          {.v = vol_mute } },
	{ Mod1Mask|ControlMask,         XK_l,      spawn,          {.v = mus_next } },
	{ Mod1Mask|ControlMask,         XK_h,      spawn,          {.v = mus_prev } },
	{ Mod1Mask|ControlMask,         XK_p,      spawn,          {.v = mus_toggle } },

	{ MODKEY|ControlMask,           XK_t,      spawn,          {.v = change_theme } },

	{ MODKEY|ShiftMask,             XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_j,      movestack,      {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_k,      movestack,      {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_h,      incnmaster,     {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_l,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.01} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.01} },
	{ MODKEY|ShiftMask,             XK_Return, zoom,           {0} },
	{ MODKEY|ShiftMask,             XK_f,      togglefullscr,  {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY,                       XK_w,      killclient,     {0} },
	{ MODKEY,                       XK_space,  cyclelayout,    {.i = +1} },
	{ MODKEY|ShiftMask,             XK_space,  cyclelayout,    {.i = -1} },
	{ MODKEY|ShiftMask,             XK_t,      togglefloating, {0} },
	{ MODKEY,                       XK_Down,   moveresize,     {.v = "0x 25y 0w 0h" } },
	{ MODKEY,                       XK_Up,     moveresize,     {.v = "0x -25y 0w 0h" } },
	{ MODKEY,                       XK_Right,  moveresize,     {.v = "25x 0y 0w 0h" } },
	{ MODKEY,                       XK_Left,   moveresize,     {.v = "-25x 0y 0w 0h" } },
	{ MODKEY|ShiftMask,             XK_Down,   moveresize,     {.v = "0x 0y 0w 25h" } },
	{ MODKEY|ShiftMask,             XK_Up,     moveresize,     {.v = "0x 0y 0w -25h" } },
	{ MODKEY|ShiftMask,             XK_Right,  moveresize,     {.v = "0x 0y 25w 0h" } },
	{ MODKEY|ShiftMask,             XK_Left,   moveresize,     {.v = "0x 0y -25w 0h" } },
	{ MODKEY|ControlMask,           XK_Up,     moveresizeedge, {.v = "t"} },
	{ MODKEY|ControlMask,           XK_Down,   moveresizeedge, {.v = "b"} },
	{ MODKEY|ControlMask,           XK_Left,   moveresizeedge, {.v = "l"} },
	{ MODKEY|ControlMask,           XK_Right,  moveresizeedge, {.v = "r"} },
	{ MODKEY|ControlMask|ShiftMask, XK_Up,     moveresizeedge, {.v = "T"} },
	{ MODKEY|ControlMask|ShiftMask, XK_Down,   moveresizeedge, {.v = "B"} },
	{ MODKEY|ControlMask|ShiftMask, XK_Left,   moveresizeedge, {.v = "L"} },
	{ MODKEY|ControlMask|ShiftMask, XK_Right,  moveresizeedge, {.v = "R"} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_r,      quit,           {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
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

