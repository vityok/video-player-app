#include <iostream>
#include <string>
#include "video-player-app.hpp"

int main(int argc, char** argv) {
  auto app = VideoPlayerApp::create();
  return app->run(argc, argv);
}
