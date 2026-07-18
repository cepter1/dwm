/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 2;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "DejaVu Sans Mono:size=10" };
static const char dmenufont[]       = "DejaVu Sans Mono:size=10";
static const char col_gray1[]       = "#000000";	/* main color of bar */
static const char col_gray2[]       = "#000000";	/* border color of inactive window */
static const char col_gray3[]       = "#FFFFFF";	/* font color of inactive desktop # */
static const char col_gray4[]       = "#FFFFFF";	/* font color of active desktop # & currently using application */
static const char col_cyan[]        = "#333333";	/* selected tag/window title color */
static const char col_selborder[]   = "#333333"; 	/* active window border */

static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_cyan,  col_selborder  },
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
	{ "Firefox",  NULL,       NULL,       1 << 8,       0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */
static const int refreshrate = 120;  /* refresh rate (per second) for client move/resize */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask /* Super/Windows key */

/*
 * For each numbered tag:
 * Super+[1-9]            = view only that tag
 * Super+Ctrl+[1-9]       = add/remove that tag from the current view
 * Super+Shift+[1-9]      = move the focused window to that tag
 * Super+Ctrl+Shift+[1-9] = add/remove that tag on the focused window
 */
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, /* view this tag */ \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, /* toggle this tag in the current view */ \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, /* move focused window to this tag */ \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} }, /* toggle this tag on focused window */

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
#include <X11/XF86keysym.h>
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[]   = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *termcmd[]    = { "st", NULL };
static const char *upvol[]   = { "/usr/bin/pulsemixer", "--change-volume", "+5", NULL };
static const char *downvol[] = { "/usr/bin/pulsemixer", "--change-volume", "-5", NULL };
static const char *mutevol[] = { "/usr/bin/pulsemixer", "--toggle-mute", NULL };
static const char *brupcmd[]    = { "brightnessctl", "set", "5%+", NULL };
static const char *brdowncmd[]  = { "brightnessctl", "set", "5%-", NULL };
static const char *filemgcmd[]  = { "st", "lf", NULL };
static const char *browsercmd[] = { "librewolf", NULL };
static const char *lockcmd[]    = { "slock", NULL };
static const char *voluicmd[]   = { "st", "pulsemixer", NULL };
static const char *mailclicmd[] = { "st", "aerc", NULL };
static const char *screenshot[] = {
    "sh", "-c",
    "dir=\"$HOME/Pictures/Screenshots\"; mkdir -p \"$dir\"; file=\"$dir/$(date '+%Y-%m-%d_%H-%M-%S').png\"; maim --select \"$file\" && xclip -selection clipboard -t image/png -i \"$file\"",
    NULL
};

static const Key keys[] = {
	/* modifier                     key        function        argument               description */
	{ MODKEY,                       XK_d,      spawn,          {.v = dmenucmd } },     /* Super+d: open dmenu application launcher */
	{ MODKEY,                       XK_Return, spawn,          {.v = termcmd } },      /* Super+Enter: open st terminal */
	{ MODKEY,                       XK_e,      spawn,          {.v = filemgcmd } },    /* Super+e: open lf file manager in st */
	{ MODKEY,                       XK_w,      spawn,          {.v = browsercmd } },   /* Super+w: open LibreWolf */
	{ MODKEY,                       XK_l,      spawn,          {.v = lockcmd } },      /* Super+l: lock the X session with slock */
	{ MODKEY,                       XK_s,      spawn,          {.v = voluicmd } },     /* Super+s: open pulsemixer in st */
	{ MODKEY,                       XK_m,      spawn,          {.v = mailclicmd } },   /* Super+m: open aerc in st */

	{ MODKEY,                       XK_b,      togglebar,      {0} },                  /* Super+b: show or hide the dwm bar */
	{ MODKEY,                       XK_z,      focusstack,     {.i = +1 } },           /* Super+z: focus next window in the stack */
	{ MODKEY|ShiftMask,             XK_z,      focusstack,     {.i = -1 } },           /* Super+Shift+z: focus previous window */
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },           /* Super+i: add one window to master area */
	{ MODKEY,                       XK_o,      incnmaster,     {.i = -1 } },           /* Super+o: remove one window from master area */
	{ MODKEY,                       XK_j,      setmfact,       {.f = -0.05} },         /* Super+j: shrink master area by 5% */
	{ MODKEY,                       XK_k,      setmfact,       {.f = +0.05} },         /* Super+k: enlarge master area by 5% */
	{ MODKEY,                       XK_x,      zoom,           {0} },                  /* Super+x: swap focused window with master */
	{ MODKEY,                       XK_Tab,    view,           {0} },                  /* Super+Tab: return to previous tag view */
	{ MODKEY,                       XK_q,      killclient,     {0} },                  /* Super+q: close focused window */
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },   /* Super+t: use tiled layout */
	{ MODKEY|ShiftMask,             XK_f,      setlayout,      {.v = &layouts[1]} },   /* Super+Shift+f: use floating layout */
	{ MODKEY|ShiftMask,             XK_m,      setlayout,      {.v = &layouts[2]} },   /* Super+Shift+m: use monocle layout */
	{ MODKEY,                       XK_space,  setlayout,      {0} },                  /* Super+Space: toggle current/previous layout */
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },                 /* Super+Shift+Space: tile/float focused window */
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },          /* Super+0: view all tags together */
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },          /* Super+Shift+0: assign focused window to all tags */
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },           /* Super+,: focus previous monitor */
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },           /* Super+.: focus next monitor */
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },           /* Super+Shift+,: move window to previous monitor */
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },           /* Super+Shift+.: move window to next monitor */
	{ MODKEY,                       XK_f,      togglefullscr,  {0} },                  /* Super+f: toggle fullscreen on focused window */

	TAGKEYS(                        XK_1,                      0)                       /* generate the four tag-1 bindings above */
	TAGKEYS(                        XK_2,                      1)                       /* generate the four tag-2 bindings above */
	TAGKEYS(                        XK_3,                      2)                       /* generate the four tag-3 bindings above */
	TAGKEYS(                        XK_4,                      3)                       /* generate the four tag-4 bindings above */
	TAGKEYS(                        XK_5,                      4)                       /* generate the four tag-5 bindings above */
	TAGKEYS(                        XK_6,                      5)                       /* generate the four tag-6 bindings above */
	TAGKEYS(                        XK_7,                      6)                       /* generate the four tag-7 bindings above */
	TAGKEYS(                        XK_8,                      7)                       /* generate the four tag-8 bindings above */
	TAGKEYS(                        XK_9,                      8)                       /* generate the four tag-9 bindings above */

	{ 0,                            XF86XK_AudioLowerVolume, spawn, {.v = downvol } }, /* volume-down key: decrease volume by 5% */
	{ 0,                            XF86XK_AudioMute,        spawn, {.v = mutevol } }, /* mute key: toggle audio mute */
	{ 0,                            XF86XK_AudioRaiseVolume, spawn, {.v = upvol   } }, /* volume-up key: increase volume by 5% */
	{ 0,                            XF86XK_MonBrightnessUp,  spawn, {.v = brupcmd } }, /* brightness-up key: increase brightness by 5% */
	{ 0,                            XF86XK_MonBrightnessDown,spawn, {.v = brdowncmd} }, /* brightness-down key: decrease brightness by 5% */
	{ MODKEY|ShiftMask,             XK_e,      quit,           {0} },                  /* Super+Shift+e: quit dwm/end X session */
	{ MODKEY|ShiftMask,             XK_s,      spawn,          {.v = screenshot} },    /* Super+Shift+s: select, save, and copy screenshot */
};


/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkStatusText,	0,		Button2,	spawn,		{.v = filemgcmd } },
	{ ClkStatusText,	0,		Button2,	spawn,		{.v = browsercmd } },
	{ ClkStatusText,	0,		Button2,	spawn,		{.v = lockcmd } },
	{ ClkStatusText,	0,		Button2,	spawn,		{.v = voluicmd } },
	{ ClkStatusText, 	0, 		Button2, 	spawn, 		{.v = mailclicmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};
