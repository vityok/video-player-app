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
  _manager->add_video_player_status_listener(*this);
}


void VideoPlayerAppWindow::open_file_view(const Glib::RefPtr<Gio::File>& file)
{
  std::cout << "opening file: " << file->get_path() << std::endl;
  _manager->play_file(file->get_path());
}


void VideoPlayerAppWindow::handle_exited(int exit_status)
{
  std::cout << "video player exited with status: " << exit_status << std::endl;
  hide();
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
    _manager->seek_back_some();
    return true;
  } else if (key_event->keyval == GDK_KEY_Right) {
    _manager->seek_some();
    // step right
    return true;
  } else if (key_event->keyval == GDK_KEY_Up) {
    // step back
    _manager->seek_many();
    return true;
  } else if (key_event->keyval == GDK_KEY_Down) {
    // step back
    _manager->seek_back_many();
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
