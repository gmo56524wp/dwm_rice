/* See LICENSE file for copyright and license details. */

/* volume control */
#include <X11/XF86keysym.h>
static const char *upvol[]   = { "/usr/bin/pactl", "set-sink-volume", "0", "+10%",     NULL };
static const char *downvol[] = { "/usr/bin/pactl", "set-sink-volume", "0", "-10%",     NULL };
static const char *mutevol[] = { "/usr/bin/pactl", "set-sink-mute",   "0", "toggle",  NULL };

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const Gap default_gap        = {.isgap = 1, .realgap = 10, .gappx = 10};
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 0;        /* 0 means bottom bar */
static const char *fonts[]          = { "SourceCodePro:size=15", "fontawesome:size=25" };
static const char dmenufont[]       = "SourceCodePro:size=12";

static const char col_gray1[]       = "#3B4252";
static const char col_gray2[]       = "#D8DEE9";
static const char col_gray3[]       = "#8FBCBB";
static const char col_gray4[]       = "#3B4252";
static const char col_cyan[]        = "#5E81AC";
static const char col_blue[]       =  "#88c0D0";
static const char col_orange[]      = "#D08770";
static const char col_green[]       = "#A3BE8c";
static const char col_pink[]        = "#EBCB8B";
static const char col_red[]         = "#b38EAD";
static const char *colors[][3]      = {
	/*					fg          bg          border   */
	[SchemeNorm] =	 { col_gray3,  col_gray1,  col_gray1 },
	[SchemeSel]  =	 { col_gray2,  col_cyan,   col_pink  },
	[Schemeblue] =	 { col_blue,   col_gray1,  col_gray1 },
	[Schemegreen]=	 { col_green,  col_gray1,  col_gray1 },
	[Schemeorange]=	 { col_orange, col_gray1,  col_gray1 },
	[Schemepink]=	 { col_pink,   col_gray1,  col_gray1 },
	[Schemered]=	 { col_red,    col_gray1,  col_gray1 },
};

/* tagging */
static const char *tags[] = { "", "", "", "" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       0,            1,           -1 },
	{ "Firefox",  NULL,       NULL,       1 << 8,       0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[t]",      tile },    /* first entry is default */
	{ "[f]",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
	{ "|M|",      centeredmaster },
	{ ">M>",      centeredfloatingmaster },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/usr/bin/alacritty", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-c", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *termcmd[]  = { "alacritty", NULL };
static const char *bravecmd[]  = { "brave", NULL };

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_F7,     spawn,          {.v = downvol } },
	{ MODKEY,                       XK_F6,     spawn,		   {.v = mutevol } },
	{ MODKEY,                       XK_F8,     spawn,          {.v = upvol   } },
	{ MODKEY,                       XK_Return, spawn,          {.v = termcmd } },
	TAGKEYS(                        XK_7,                      0)
	TAGKEYS(                        XK_8,                      1)
	TAGKEYS(                        XK_9,                      2)
	TAGKEYS(                        XK_0,                      3)
	{ MODKEY,                       XK_y,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_u,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_i,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_o,      setlayout,      {.v = &layouts[3]} },
	{ MODKEY,                       XK_p,      setlayout,      {.v = &layouts[4]} },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_semicolon,  zoom,       {0} },
	{ MODKEY,                       XK_apostrophe,      killclient,     {0} },
	{ MODKEY,                       XK_n,      togglebar,      {0} },
	{ MODKEY,                       XK_m,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_comma,  incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_period, spawn,          {.v = bravecmd } },
	{ MODKEY,                       XK_slash,  spawn,          {.v = dmenucmd } },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY,                       XK_a,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_Return, setlayout,      {0} },
	{ MODKEY,                       XK_w,      focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_r,      focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	{ MODKEY,                       XK_q,      togglefloating, {0} },
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
	{ MODKEY,                       XK_minus,  setgaps,        {.i = -5 } },
	{ MODKEY,                       XK_equal,  setgaps,        {.i = +5 } },
	{ MODKEY|ShiftMask,             XK_minus,  setgaps,        {.i = GAP_RESET } },
	{ MODKEY|ShiftMask,             XK_equal,  setgaps,        {.i = GAP_TOGGLE} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
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

