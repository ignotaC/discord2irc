
This program is very simple. It is a script
connecting two programs. That redirects discord server
to specified irc channel.

1. Quick overlook discord part
2. Quick overlook irc part
3. Installation

## Quick overlook discord part
##  1  ################################################################
Discord program:  logdisc wraped with logdisc.sh
It redirects data from discord server on stdout.
It is inside scandiscord dir and has it's own makefile
so you can always compile it separatly.

Notice, logging discord server, not channel.

Discord calls their servers guilds, but we will call them servers.
Each server has usually few channels. Everything written on those
will be redirected to stdin in form of:
#<channel>|<user>:<message>LF
Where LF is linefeed.

Dependencies: curl lib+inc, gmake, git, cc, pkg-config
It will git clone source code of concord which is a discord C lib.
It's not a submodule because since discord changes so often,
it is safer to just get the newest uptodate lib.

Used files in data directory: config.json


## Quick overlook irc part
##  2  ################################################################
Irc program: ircbot wrapper runircbot.sh
It writes data from stdin to irc server->single channel
It is inside writeirc dir. It also has it's own makefile
so also can be compiled and used separatly
It can't connect on ports with SSL. No encryption on
data stream.

Dependencies: make, cmake, cc, ksh, git
Submodules: ignota, smallsoft

Used files in data directory: irc.conf msgaftercon


## Installation
##  3  ################################################################

DEPENDENCIES

First of all you need to get dependencies:
-> ksh
-> cc ( c compiler vide clang or gcc )
-> cmake
-> pkg-config
-> curl library + headers ( dev )
-> git
-> gmake ( a must for discord->concord )

SETING UP DIRECTORIES:  !!! CRUCIAL !!!

Before you start doing anything,
you should create directory for this future program.
git clone in it so directory with the program is created
and create data directory. This would look like this:

mkdir d2i
cd d2i
git clone <link to this rpository> # this will create discord2irc directory
mkdir data

IT IS VERY IMPORTANT TO NEVER CHNAGE PROGRAM DIRECTORIES.
THEY WORK ONLY INSIDE THIS SETUP.

BUILDING

enter dicord2irc directory and run make:
cd discord2irc
make

This should build everything. Now program is ready to use.
But you still need to fill the config files

If you would want to remove all installed files run:
make clear

There is also make clean which does the same thing make clear does.

FILLING CONFIG FILES:

We have three config files make has moved to data dir
( else it would not build )

irc.conf msgaftercon config.json

I will show how those files look like and discribe which parts you edit.


1. config.json - discord config file

############# start 
{
  "logging": {
    "level": "info",
    "filename": "/dev/null",
    "quiet": false,
    "overwrite": false,
    "use_color": false,
    "http": {
      "enable": false,
      "filename": "http.empty"
    },
    "disable_modules": ["WEBSOCKETS", "USER_AGENT", "DISCORD_HTTP" ]
  },
  "discord": {
    "token": "<token code you need to enter here>",
    "default_prefix": {
      "enable": false,
      "prefix": "YOUR-COMMANDS-PREFIX"
    }
  }
}
############ end

You only edit here one variable.
"token"
You get this on bot discord developer panel.
I will write how to do it later.


2. irc.conf - irc configuration file

############# start 
domain: irc.libera.chat
port: 6667
channel: #channel
nick: 
realname: bot
password:
############# end

This is configuration file of channel on irc server where you want to redirect
discord server output.

You only edit data after xxx:
Spaces are ignored. So port: 1000 is same as port:1000.
Note you can't have whitespace characters in password.
Order does not matter

First we need irc network domain where channel for discord redirection is
domain: < irc server domain name >  

Than port of that irc server
port: < irc server port >

Channel we will redirect stuff to
channel: < channel on irc network >

Our nick on that server for example on freenode
our bot can have an account which we cloak before entering channels
nick: < nick on irc network we will use, or account we have there >

Realname, well just write here what ever you want, default is bot
realname: < bot name >

Password that can be used to log in on network account. This will be
discussed more in third last config file.
password: < some password not using spaces >

You can add your own arguments, a line example:
liberapass: liberapass

############# start

PRIVMSG nickserv :IDENTIFY &nick &password

############# end

Every line in this file is parsed.
Empty lines get ignored.
Anything with & before is treated as a variable and is swaped with
value that is stored inside.

So parser when encounters &nick looks for
nick: <somename>  inside irc.conf
and swaps &name to <somename>

After that and establishing connection  line is sent to ircserver.
This happens before joining channels.

Example line is a simple way of sending a authentication request on
libera network. So when you enable having cloak your bot will stay
hidden.

Notice in example line and remeber. ':' is part of irc  message protocol
so is PRIVMSG. 

SETTING UP ACCOUNTS FOR BOTS ON IRC AND DISCORD

IRC

This is fairly simple. You need to find irc sevrers domain name and port.
Every irc has support channel named by netowrk name where they can help
you out. Some irc networks don't even have user/login you simply connect
by nick. For example ircnet. Others like libera allow you to have
user account. 

So on libera you would create account in a manner like that:
irssi -c irc.libera.chat
/nick <nick I want>
/msg nickserv REGISTER <password> <email>

DISCORD

On discord you need to simply create your account. Rest is not that simple.
Documentation for discord is complete garbage, everything is hidden in
gui borthel and no matter how good your intuition is you have zero chance to
do it on your own without losing a lot of time for some basic stuff like
check box or find token.

All steps here are for using webbrowser discord page.
You don't need to install their app. Which is probably as
bad as everything else. 

1. You create your account.

Type Discord in your web browser and move inside, find registration for
account, register. Log in, this is important for next steps. Keep being
loged in.
Fairly easy as far. once you do that move to point 2.

2. Tell discord you want to have a bot

Yes you literaly have to tell fucking discordm
that - you want to have a bot. 
Discord is very affraid of bots and their dumb fuck gamer population,
need to be protected from bots ;-)

Don't try to find where that add bot button is in your discord panel.
Don't waste time and write in your browser: discord developer portal.

Before you enter it KEEP BEING LOGGED on the discord page in your second
browser tab.

k so you are in the developer panel. And there should be a menu.
Applications. Click that ( you should by default start in it ).
And then you will have on that application card: New application

Click that and create your bot. Ones you do that,
stay in that stupid panel.

3. Get token for bot

If you stayed in the application card - a box/div or what ever, 
an icon with your bot name appeared. Click that

You are now in your bot menu - pick from menu: Bot
On Bot card you need to create/reset bot token.
Do that and save the token which is string of chars.
Save that to your irc.conf file, into this line it goes:

    "token": "<token code you need to enter here>",

Because safety you can only see that token when you create it.

4. Permissions

This is real cancer and for now i feel like endig,
the documentation will be updated later


















