#include <iostream>
#include <list>
#include <string>

#include "video-player-app.hpp"

using std::string;

const string OmxPlayerProxy::PROG_NAME = "omxplayer";

void OmxPlayerProxy::launch_player(const string& fname) {
  std::cout << "omxplayer::launch_player" << std::endl;

  std::list<string> args = { "-b", fname };

  spawn_process(PROG_NAME, args);
}
