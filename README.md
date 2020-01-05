video-player-app
----------------

GUI interface for the OMX Player.

Uses GTK for UI, OMX Player for playing video on Raspberry Pi, MPlayer
on the desktop Linux.

Goal is to create a lightweight and fast GUI for the OMXPlayer app on
Raspberry Pi. It will not have the overhead of Python with its bloat
of libraries and bindings, and at the same time will offer a minimal
UI to conveniently playback videos with hardware acceleration and high
quality by the OMX Player.

Work in progress.

Depends on gtkmm library version 3.0 or higher. To install run:

    sudo apt-get install libgtkmm-3.0-dev

If you are building on a Debian-based system (Ubuntu or Raspbian).

There is still plenty to do before calling it a "release", but this
app does what it was created to do and is already rather useful.

What it does
------------

This is a tiny GTK-based application that does its best to launch the
actual OMX Player application in background.

It also creates a GUI window in the window manager so that it can
intercept keypress events and relay them to the running omxplayer
process.

Thus, it becomes possible to associate video file types with this
application for convenient video playback right from the file browser.

Links
-----

* [OMX Player home](https://github.com/popcornmix/omxplayer)
