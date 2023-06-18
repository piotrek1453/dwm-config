/* See LICENSE file for copyright and license details. */
/* appearance */
#define ICONSIZE 16   /* icon size */
#define ICONSPACING 5 /* space between icon and title */

static unsigned int borderpx  = 3;        /* border pixel of windows */
static unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no standard bar */
static const int topbar             = 1;        /* 0 means standard bar at bottom */
static const char statussep         = ';';      /* separator between status bars */
static const int extrabar           = 1;        /* 0 means no extra bar */          
static char font[]            = "Lucida Console Nerd Font Mono:size=16:antialias=true:autohint=true";
static char dmenufont[]       = "Lucida Console Nerd Font Mono:size=16:antialias=true:autohint=true";
static const char *fonts[]          = { "Lucida Console Nerd Font Mono:size=16:antialias=true:autohint=true",
  "Symbols Nerd Font:size=16:antialias=trueautohint=true",
  "JoyPixels:pixelsize=16"};
static char normbgcolor[]           = "#e1140a";
static char normbordercolor[]       = "#00ff00";
static char normfgcolor[]           = "#ffffff";
static char selfgcolor[]            = "#ffffff";
static char selbordercolor[]        = "#0000ff";
static char selbgcolor[]            = "#000000";
static char *colors[][3] = {
  /*               fg           bg           border   */
  [SchemeNorm] = { normfgcolor, normbgcolor, normbordercolor },
  [SchemeSel]  = { selfgcolor,  selbgcolor,  selbordercolor  },
};
static const unsigned int gappx     = 5;        /* gaps between windows */

/* tagging */
static const char *tags[] = { "", "2", "3", "", "󰇮", "󰍡"};

static const Rule rules[] = {
  /* xprop(1):
   *	WM_CLASS(STRING) = instance, class
   *	WM_NAME(STRING) = title
   */
  /* class      instance    title       tags mask     isfloating   monitor */
  { "Gimp",     NULL,       NULL,       0,            1,           -1 },
  { "firefox",  NULL,       NULL,       1 << 3,       0,           -1 },
  { "thunderbird", "Mail",   NULL,       1 << 4,       0,           -1 },
  { "TelegramDesktop", "telegram-desktop",   NULL,       1 << 5,       0,           -1 },
  { "Caprine", "caprine",   NULL,       1 << 5,       0,           -1 }
};

/* layout(s) */
static float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static int nmaster     = 1;    /* number of clients in master area */
static int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
  /* symbol     arrange function */
  { "[]=",      tile },    /* first entry is default */
  { "><>",      NULL },    /* no layout function means floating behavior */
  { "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/usr/local/bin/st", "-c", cmd, NULL } }
#include <X11/XF86keysym.h>

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbordercolor, "-sf", selfgcolor, NULL };
static const char *termcmd[]  = { "st", NULL };
static const char *Firefox[] = { "firefox", NULL };
static const char *pwrmenu[] = { "rofi", "-show", "power-menu", "-modi", "power-menu:/home/juchap/.scripts/rofi-power-menu", NULL };
//volume control
static const char *upvol[] = {"/usr/bin/pactl", "set-sink-volume", "0", "+5%", NULL};
static const char *downvol[] = {"/usr/bin/pactl", "set-sink-volume", "0", "-5%", NULL};
static const char *mute[] = {"/usr/bin/pactl", "set-sink-mute", "0", "toggle", NULL};

static const char *brupcmd[] = { "brightnessctl", "set", "10%+", NULL };
static const char *brdowncmd[] = { "brightnessctl", "set", "10%-", NULL };

static const char *flameshot[]= {"flameshot", "gui", NULL};
#include "shiftview.c"
/*
 * Xresources preferences to load at startup
 */
ResourcePref resources[] = {
  { "font",               STRING,  &font },
  { "dmenufont",          STRING,  &dmenufont },
  { "normbgcolor",        STRING,  &normbgcolor },
  { "normbordercolor",    STRING,  &normbordercolor },
  { "normfgcolor",        STRING,  &normfgcolor },
  { "selbgcolor",         STRING,  &selbgcolor },
  { "selbordercolor",     STRING,  &selbordercolor },
  { "selfgcolor",         STRING,  &selfgcolor },
  { "borderpx",          	INTEGER, &borderpx },
  { "snap",           		INTEGER, &snap },
  { "showbar",          	INTEGER, &showbar },
  { "topbar",           	INTEGER, &topbar },
  { "nmaster",          	INTEGER, &nmaster },
  { "resizehints",       	INTEGER, &resizehints },
  { "mfact",      	    	FLOAT,   &mfact },
};

static const Key keys[] = {
  /* modifier                     key                            function                       argument */
  { MODKEY,                       XK_p,                          spawn,                         {.v = dmenucmd } },
  { MODKEY|ShiftMask,             XK_Return,                     spawn,                         {.v = termcmd } },
  // { MODKEY,                       XK_b,                          togglebar,                     {0} },
  { MODKEY,                       XK_j,                          focusstack,                    {.i = +1 } },
  { MODKEY,                       XK_k,                          focusstack,                    {.i = -1 } },
  { MODKEY,                       XK_i,                          incnmaster,                    {.i = +1 } },
  { MODKEY,                       XK_d,                          incnmaster,                    {.i = -1 } },
  { MODKEY,                       XK_h,                          setmfact,                      {.f = -0.05} },
  { MODKEY,                       XK_l,                          setmfact,                      {.f = +0.05} },
  { MODKEY,                       XK_Return,                     zoom,                          {0} },
  { MODKEY,                       XK_Tab,                        view,                          {0} },
  { MODKEY|ShiftMask,             XK_q,                          killclient,                    {0} },
  { MODKEY,                       XK_t,                          setlayout,                     {.v = &layouts[0]} },
  { MODKEY,                       XK_f,                          setlayout,                     {.v = &layouts[1]} },
  { MODKEY,                       XK_m,                          setlayout,                     {.v = &layouts[2]} },
  { MODKEY,                       XK_space,                      setlayout,                     {0} },
  { MODKEY|ShiftMask,             XK_space,                      togglefloating,                {0} },
  { MODKEY,                       XK_0,                          view,                          {.ui = ~0 } },
  { MODKEY|ShiftMask,             XK_0,                          tag,                           {.ui = ~0 } },
  { MODKEY,                       XK_comma,                      focusmon,                      {.i = -1 } },
  { MODKEY,                       XK_period,                     focusmon,                      {.i = +1 } },
  { MODKEY|ShiftMask,             XK_comma,                      tagmon,                        {.i = -1 } },
  { MODKEY|ShiftMask,             XK_period,                     tagmon,                        {.i = +1 } },
  { MODKEY,                       XK_minus,                      setgaps,                       {.i = -1 } },
  { MODKEY,                       XK_equal,                      setgaps,                       {.i = +1 } },
  { MODKEY|ShiftMask,             XK_equal,                      setgaps,                       {.i = 0  } },
  { MODKEY|ControlMask,           XK_q,                          quit,                          {.i = 23} },
  { MODKEY,			                  XK_b,                          shiftview,	                    {.i = -1 } },
  { MODKEY,			                  XK_n,	                         shiftview,	                    {.i = +1 } },
  { MODKEY,			                  XK_F2,	                       spawn,	                        {.v = Firefox } },
  { MODKEY|ShiftMask,		          XK_F10,	                       spawn,	                        {.v = pwrmenu } },
  { 0,                            XF86XK_AudioRaiseVolume,	     spawn,                         {.v = upvol }},
  { 0,                            XF86XK_AudioLowerVolume,	     spawn,                         {.v = downvol }},
  { 0,                            XF86XK_AudioMute,	             spawn,                         {.v = mute }},
  { 0,                            XF86XK_MonBrightnessUp,        spawn,                         {.v = brupcmd} },
  { 0,                            XF86XK_MonBrightnessDown,      spawn,                         {.v = brdowncmd} },
//screenshot
  {MODKEY|ShiftMask,              XK_s,                          spawn,                         {.v = flameshot }},
  {MODKEY|ShiftMask,              XK_S,                          spawn,                         {.v = flameshot }},
  TAGKEYS(                        XK_1,                                                         0)
  TAGKEYS(                        XK_2,                                                         1)
  TAGKEYS(                        XK_3,                                                         2)
  TAGKEYS(                        XK_4,                                                         3)
  TAGKEYS(                        XK_5,                                                         4)
  TAGKEYS(                        XK_6,                                                         5)
  TAGKEYS(                        XK_7,                                                         6)
  TAGKEYS(                        XK_8,                                                         7)
  TAGKEYS(                        XK_9,                                                         8)
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
  /* click                event mask      button          function        argument */
  { ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
  { ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
  { ClkWinTitle,          0,              Button2,        zoom,           {0} },
  { ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
  { ClkExBarLeftStatus,   0,              Button2,        spawn,          {.v = termcmd } },
  { ClkExBarMiddle,       0,              Button2,        spawn,          {.v = termcmd } },
  { ClkExBarRightStatus,  0,              Button2,        spawn,          {.v = termcmd } },  
  { ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
  { ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
  { ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
  { ClkTagBar,            0,              Button1,        view,           {0} },
  { ClkTagBar,            0,              Button3,        toggleview,     {0} },
  { ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
  { ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

