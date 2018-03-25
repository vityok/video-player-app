A very basic and simple omxplayer GTK GUI app written in the Vala
language for the Raspbian Raspberry Pi operating system.

Work in progress.


Goals
=====

* Light-weight
* As little dependencies as possible
* Easy to install
* Easy to use


How it works
============

Runs the standard omxplayer in a spawned child process, sends key
presses via the stdio pipes.