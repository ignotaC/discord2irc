
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
-> cc
-> make
-> cmake
-> pkg-config
-> curl library + headers ( dev )
-> git
-> gmake

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

We have three config files make has moved to data dir ( else it would not build )
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
    "token": "MTE5NzYwMjI0Mzk5ODkyMDczNg.GyhCbr.b1TPfdwvBu_dI1Nbo4ZYHw609Z8f93vThh3eBA",
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
discussed more with third last config file.
password: < some password not using spaces >




