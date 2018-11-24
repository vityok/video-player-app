#include <glibmm.h>

#include <iostream>
#include <list>
#include <string>

#include "video-player-app.hpp"

using std::string;

const string OmxPlayerProxy::PROG_NAME = "omxplayer";


// todo: it might be more convenient and reasonable to use the DBus
// API for interaction with the running OMX Player application
//
// The API reference can be found at:
//
// http://transit.iut2.upmf-grenoble.fr/doc/glibmm-2.4/reference/html/group__DBus.html
//
// And example application at:
//
// https://github.com/GNOME/glibmm/tree/master/examples/dbus
//
// The C API is documented at:
//
// https://developer.gnome.org/gio/stable/gdbus-convenience.html
//
// And a tutorial at:
//
// https://dbus.freedesktop.org/doc/dbus-tutorial.html
//
// However, interacting "old school" style via pipes appears to be
// much easier to implement and also more reliable and doesn't impose
// requirement to get into DBus programming.
//
// So, sticking with the pipes for a while until DBus stuff is sorted
// out.
void OmxPlayerProxy::launch_player(const string& fname)
{
  std::cout << "omxplayer::launch_player" << std::endl;

  std::list<string> args = { "-b", fname };

  spawn_process(PROG_NAME, args);

}

// Key bindings to control omxplayer while playing:

// 1           decrease speed
// 2           increase speed
// <           rewind
// >           fast forward
// z           show info
// j           previous audio stream
// k           next audio stream
// i           previous chapter
// o           next chapter
// n           previous subtitle stream
// m           next subtitle stream
// s           toggle subtitles
// w           show subtitles
// x           hide subtitles
// d           decrease subtitle delay (- 250 ms)
// f           increase subtitle delay (+ 250 ms)
// q           exit omxplayer
// p / space   pause/resume
// -           decrease volume
// + / =       increase volume
// left arrow  seek -30 seconds
// right arrow seek +30 seconds
// down arrow  seek -600 seconds
// up arrow    seek +600 seconds


void OmxPlayerProxy::pause()
{
  // DBus player method: Pause

  // Pause the video. If the video is playing, it will be paused, if
  // it is paused it will stay in pause (no effect).
}


void OmxPlayerProxy::resume()
{
}


void OmxPlayerProxy::toggle_pause()
{
  pipe_msg("p");
}

// hex codes borrowed from the TBO Player sources
void OmxPlayerProxy::seek_some()
{
  pipe_msg("\x1b\x5b\x43");
}

void OmxPlayerProxy::seek_back_some()
{
  pipe_msg("\x1b\x5b\x44");
}

void OmxPlayerProxy::seek_many()
{
  pipe_msg("\x1b\x5b\x42");
}

void OmxPlayerProxy::seek_back_many()
{
  pipe_msg("\x1b\x5b\x41");
}

void OmxPlayerProxy::stop()
{
  pipe_msg("q");
  this->VideoPlayerProxy::stop();
}


// eof
