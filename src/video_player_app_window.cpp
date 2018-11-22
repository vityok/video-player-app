#include <gtkmm.h>

#include <iostream>
#include <memory>
#include <string>

#include "video-player-app.hpp"

using std::string;
using std::unique_ptr;


VideoPlayerAppWindow::VideoPlayerAppWindow()
  : Gtk::ApplicationWindow(),
    _manager(unique_ptr<VideoPlayerManager>(new VideoPlayerManager()))
{
}


void VideoPlayerAppWindow::open_file_view(const Glib::RefPtr<Gio::File>& file)
{
  std::cout << "opening file: " << file->get_path() << std::endl;
  _manager->play_file(file->get_path());
}


bool VideoPlayerAppWindow::on_key_press_event(GdkEventKey* key_event)
{
  std::cout << gdk_keyval_name (key_event->keyval) << std::endl;

  if (key_event->keyval == GDK_KEY_space) {
    // pause
    _manager->toggle_pause();
    return true;
  } else if (key_event->keyval == GDK_KEY_Left) {
    // step back
    return true;
  } else if (key_event->keyval == GDK_KEY_Right) {
    // step right
    return true;
  } else if (key_event->keyval == GDK_KEY_Escape
	     || key_event->keyval == GDK_KEY_q) {
    //close the window, when the 'esc' or 'q' key is pressed
    _manager->stop();
    hide();
    return true;
  }

  //if the event has not been handled, call the base class
  return Gtk::Window::on_key_press_event(key_event);
}
