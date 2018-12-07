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
  _player->toggle_pause();
}

void VideoPlayerManager::pause()
{
  _player->pause();
}

void VideoPlayerManager::resume()
{
  _player->resume();
}

void VideoPlayerManager::stop()
{
  _player->stop();
}

void VideoPlayerManager::seek_some()
{
  _player->seek_some();
}

void VideoPlayerManager::seek_back_some()
{
  _player->seek_back_some();
}

void VideoPlayerManager::seek_many()
{
  _player->seek_many();
}

void VideoPlayerManager::seek_back_many()
{
  _player->seek_back_many();
}

void VideoPlayerManager::add_video_player_status_listener(AVideoPlayerStatusListener& listener)
{
  _player->add_video_player_status_listener(listener);
}
