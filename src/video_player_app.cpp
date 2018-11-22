#include <gtkmm.h>

#include <iostream>
#include <memory>
#include <string>

#include "video-player-app.hpp"

using std::string;
using std::unique_ptr;

VideoPlayerApp::VideoPlayerApp() :
  Gtk::Application("org.bb.vityok.videoplayerapp", Gio::APPLICATION_HANDLES_OPEN)
{
  VideoPlayerProxy* proxy = new MplayerProxy();
  VideoPlayerManager* mgr = new VideoPlayerManager(proxy);
  _manager = unique_ptr<VideoPlayerManager>(mgr);
}

VideoPlayerApp::~VideoPlayerApp() {
}


Glib::RefPtr<VideoPlayerApp> VideoPlayerApp::create()
{
  return Glib::RefPtr<VideoPlayerApp>(new VideoPlayerApp());
}


VideoPlayerAppWindow* VideoPlayerApp::create_appwindow()
{
  auto appwindow = new VideoPlayerAppWindow();

  // Make sure that the application runs for as long this window is still open.
  add_window(*appwindow);

  // Gtk::Application::add_window() connects a signal handler to the window's
  // signal_hide(). That handler removes the window from the application.
  // If it's the last window to be removed, the application stops running.
  // Gtk::Window::set_application() does not connect a signal handler, but is
  // otherwise equivalent to Gtk::Application::add_window().

  // Delete the window when it is hidden.
  appwindow->signal_hide().connect(sigc::bind<Gtk::Window*>(sigc::mem_fun(*this,
    &VideoPlayerApp::on_hide_window), appwindow));

  return appwindow;
}

void VideoPlayerApp::on_activate()
{
  // The application has been started, so let's show a window.
  auto appwindow = create_appwindow();
  appwindow->present();
}

void VideoPlayerApp::on_open(const Gio::Application::type_vec_files& files,
  const Glib::ustring& /* hint */)
{
  // The application has been asked to open some files,
  // so let's open a new view for each one.
  VideoPlayerAppWindow* appwindow = nullptr;
  auto windows = get_windows();
  if (windows.size() > 0)
    appwindow = dynamic_cast<VideoPlayerAppWindow*>(windows[0]);

  if (!appwindow)
    appwindow = create_appwindow();

  for (const auto& file : files)
    appwindow->open_file_view(file);

  appwindow->present();
}

void VideoPlayerApp::on_hide_window(Gtk::Window* window)
{
  delete window;
}

int VideoPlayerApp::run(int argc, char** argv) {
  // currently we support just one parameter - the file name of the
  // video to play
  if (argc == 2) {
    const string fname(argv[1]);
    std::cout << "Playing back: " << std::endl;
    std::cout << "  " << fname << std::endl;
    _manager->play_file(fname);
  } else {
    std::cout << "Usage: " << std::endl;
    std::cout << "  video-player-app VIDEO_FILE" << std::endl;
  }
  return 0;
}

// ------ WINDOW
VideoPlayerAppWindow::VideoPlayerAppWindow()
: Gtk::ApplicationWindow()
{
}

void VideoPlayerAppWindow::open_file_view(const Glib::RefPtr<Gio::File>& /* file */)
{
}
