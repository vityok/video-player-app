A very basic and simple omxplayer GUI (graphical user interface) app
written in the Vala programming language for the Raspbian Raspberry Pi
operating system.

Work in progress.


Goals
=====

* Light-weight: does not require additional interpreters, run-time
  environments, etc. Binary is around 25k at the moment.

* As little dependencies as possible: in fact, no dependencies but the
  very basic Gtk, Glib, Posix and the omxplayer itself.

* Easy to install: without extenal dependencies just dropping in the
  binary must be sufficient.

* Easy to use: just list the files you want to play in the command
  line!


How it works
============

Runs the standard omxplayer in a spawned child process, sends key
presses via the stdio pipes.