#include <iostream>
#include <memory>

#include "video-player-app.hpp"

VideoPlayerManager::VideoPlayerManager(VideoPlayerProxy* player) :
  _player (std::unique_ptr<VideoPlayerProxy>(player))
{
}

VideoPlayerManager::~VideoPlayerManager() {
}

void VideoPlayerManager::play_file(const string& fname) {
  std::cout << "Manager::play_file " << fname << std::endl;
  _player->launch_player(fname);
}
