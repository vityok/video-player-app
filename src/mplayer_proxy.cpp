#include <iostream>
#include <list>
#include <string>

#include "video-player-app.hpp"

using std::string;

const string MplayerProxy::PROG_NAME = "mplayer";

void MplayerProxy::launch_player(const string& fname) {
  std::cout << "mplayer::launch_player" << std::endl;

  std::list<string> args = { fname };

  spawn_process(PROG_NAME, args);
}

void MplayerProxy::pause()
{}

void MplayerProxy::resume()
{}

void MplayerProxy::toggle_pause()
{}

void MplayerProxy::stop()
{}

void MplayerProxy::seek_some()
{}

void MplayerProxy::seek_many()
{}

void MplayerProxy::seek_back_some()
{}

void MplayerProxy::seek_back_many()
{}
