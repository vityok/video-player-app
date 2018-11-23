#include <iostream>
#include <memory>

#include "video-player-app.hpp"

VideoPlayerManager::VideoPlayerManager() :
  // it would be more appropriate to implement a video player proxy
  // factory of some sort, but this approach appears to be working, so
  // leaving it as is at the moment but should be kept in mind for
  // future improvements
#ifdef APP_OS_RASPBIAN
  _player (std::unique_ptr<VideoPlayerProxy>(new OmxPlayerProxy())),
#else
  _player (std::unique_ptr<VideoPlayerProxy>(new MplayerProxy())),
#endif
  state (0)
{
}

VideoPlayerManager::~VideoPlayerManager()
{
}

void VideoPlayerManager::play_file(const string& fname)
{
  std::cout << "Manager::play_file " << fname << std::endl;
  _player->launch_player(fname);
}

void VideoPlayerManager::toggle_pause()
{
}

void VideoPlayerManager::pause()
{
}

void VideoPlayerManager::resume()
{
}

void VideoPlayerManager::stop()
{
}
