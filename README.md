video-player-app
----------------

Video player GUI interface.

Developed as an excercise in C++ programming. Uses GTK for UI, OMX
Player for playing Video on Raspberry Pi, MPlayer on desktop.

Goal is to create a lightweight and fast GUI for the OMXPlayer app on
Raspberry Pi. It will not have the overhead of Python with its bloat
of libraries and bindings, and at the same time will offer a minimal
UI to conveniently playback videos with hardware acceleration and high
quality by the OMX Player.

Work in progress.

Depends on gtkmm library version 3.0 or higher. To install run:

    sudo apt-get install libgtkmm-3.0-dev

If you are building on a Ubuntu (might even work on Debian) or
Raspbian system.

Links
-----

* [OMX Player home](https://github.com/popcornmix/omxplayer)