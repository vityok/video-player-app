#include <giomm.h>
#include <glibmm.h>

#include <iostream>
#include <list>
#include <string>

#include "video-player-app.hpp"

using std::string;

// root: org.mpris.MediaPlayer2

// Player Interface

// The player interface is accessible under the name
// org.mpris.MediaPlayer2.Player

// Methods

// Player interface methods can be accessed through
// org.mpris.MediaPlayer2.Player.MethodName



const string OmxPlayerProxy::PROG_NAME = "omxplayer";

// A callback to finish creating a DBus::Proxy that was asynchronously created
// for the user session's bus and then try to call the well known 'ListNames'
// method.
void
on_dbus_proxy_available(Glib::RefPtr<Gio::AsyncResult>& result)
{
  const auto proxy = Gio::DBus::Proxy::create_finish(result);

  if (!proxy) {
    std::cerr <<
      "The proxy to the user's session bus was not successfully "
      "created."
	      << std::endl;
    // loop->quit();
    return;
  }

  try {
    // The proxy's call method returns a tuple of the value(s) that the method
    // call produces so just get the tuple as a VariantContainerBase.
    proxy->call_sync("Pause");

  } catch (const Glib::Error& error) {
    std::cerr << "Got an error: '" << error.what() << "'." << std::endl;
  }

  // Connect an idle callback to the main loop to quit when the main loop is
  // idle now that the method call is finished.
  // Glib::signal_idle().connect(sigc::ptr_fun(&on_main_loop_idle));
}


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
void OmxPlayerProxy::launch_player(const string& fname)
{
  std::cout << "omxplayer::launch_player" << std::endl;

  std::list<string> args = { "-b", fname };

  spawn_process(PROG_NAME, args);

  auto connection = Gio::DBus::Connection::get_sync(Gio::DBus::BusType::BUS_TYPE_SESSION);

  // Check for an unavailable connection.
  if (!connection) {
    std::cerr << "The user's session bus is not available." << std::endl;
    return;
  }
  sleep(2);

  // Create the proxy to the bus asynchronously.

  Gio::DBus::Proxy::create(connection,
			   "org.mpris.MediaPlayer2",
			   "/org/mpris/MediaPlayer2",
			   "org.mpris.MediaPlayer2.Player",
			   sigc::ptr_fun(&on_dbus_proxy_available));

}


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
  // DBus player method: PlayPause

  // Toggles the play state. If the video is playing, it will be
  // paused, if it is paused it will start playing.

  std::cout << "OmxPlayerProxy::toggle_pause" << std::endl;

  pipe_msg("p");
}


void OmxPlayerProxy::stop()
{
  pipe_msg("q");
  this->VideoPlayerProxy::stop();
  // DBus player method: Stop

  // Stops the video. This has the same effect as Quit (terminates the
  // omxplayer instance).
}


// eof
