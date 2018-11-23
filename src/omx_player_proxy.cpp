#include <giomm.h>
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
void OmxPlayerProxy::launch_player(const string& fname) {
  std::cout << "omxplayer::launch_player" << std::endl;

  std::list<string> args = { "-b", fname };

  spawn_process(PROG_NAME, args);
}
