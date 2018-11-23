#ifndef VIDEO_PLAYER_APP
#define VIDEO_PLAYER_APP

#include <gtkmm.h>

#include <list>
#include <memory>
#include <string>

using std::string;
using std::unique_ptr;

/** A proxy responsible for launching the video player application and
 *  interfacing with it via pipes.
 */
class VideoPlayerProxy {
public:
  /** Fork-exec a video playing application, setup pipes for
   *   communication.
   */
  virtual void launch_player(const string& fname) = 0;
  virtual ~VideoPlayerProxy();

  virtual void pause() = 0;
  virtual void resume() = 0;
  virtual void toggle_pause() = 0;
  /** Stop playback, kill the video player. */
  virtual void stop() = 0;

protected:
  VideoPlayerProxy() {};

  /** It is a function responsible for forking and executing the given
   * process with the given argument list.
   */
  void spawn_process(const string& app, std::list<string>& args);

  /** Send the given message via an output pipe to the child
   *  process. */
  void pipe_msg(const string& msg);

  int pipe_out[2];
  int pipe_in[2];
  pid_t child_pid;
};

// ------------------------------------------------------

/** Proxy to the OMX Player (Raspberry Pi). */
class OmxPlayerProxy : public VideoPlayerProxy {
public:
  static const string PROG_NAME;

  void launch_player(const string& fname) override;
  void pause() override;
  void resume() override;
  void toggle_pause() override;
  /** Stop playback, kill the video player. */
  void stop() override;

};

// ------------------------------------------------------

/** Proxy to the MPlayer (Desktop). */
class MplayerProxy : public VideoPlayerProxy {
public:
  static const string PROG_NAME;

  void launch_player(const string& fname) override;
};

// ------------------------------------------------------

/** Manages the video player proxy, gets notified if it is running or down. */
class VideoPlayerManager {
public:
  VideoPlayerManager();
  ~VideoPlayerManager();

  /** Launch the video player application, normal playback. */
  void play_file(const string& fname);
  void pause();
  void resume();
  void toggle_pause();
  /** Stop playback, kill the video player. */
  void stop();

private:
  unique_ptr<VideoPlayerProxy> _player;
  int state;
};

// ------------------------------------------------------

class VideoPlayerAppWindow : public Gtk::ApplicationWindow
{
public:
  VideoPlayerAppWindow();

  void open_file_view(const Glib::RefPtr<Gio::File>& file);
  bool on_key_press_event(GdkEventKey* event) override;
private:
  unique_ptr<VideoPlayerManager> _manager;
};

// ------------------------------------------------------

/** The application itself.
 *
 * It is a singleton as there can be just one app running within a
 * single process. */
class VideoPlayerApp : public Gtk::Application {
public:

  static Glib::RefPtr<VideoPlayerApp> create();

protected:
  VideoPlayerApp();

  // Override default signal handlers:
  void on_activate() override;
  void on_open(const Gio::Application::type_vec_files& files,
	       const Glib::ustring& hint) override;

private:
  ~VideoPlayerApp();
  VideoPlayerApp(VideoPlayerApp const&) = delete;
  void operator=(VideoPlayerApp const&) = delete;

  VideoPlayerAppWindow* create_appwindow();
  void on_hide_window(Gtk::Window* window);
};

#endif
