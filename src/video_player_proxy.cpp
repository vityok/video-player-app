#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>

#include <iostream>
#include <list>
#include <string>

#include "video-player-app.hpp"

using std::string;

void VideoPlayerProxy::spawn_process(const string& prog, std::list<string>& args) {

  if (pipe(pipe_out)==-1) {
    std::cerr << "ERROR: pipe failed" << std::endl;
    return ;
  }
  if (pipe(pipe_in)==-1) {
    std::cerr << "ERROR: pipe failed" << std::endl;
    return ;
  }

  pid_t pid = fork();
  if (pid < 0) {
    std::cerr << "ERROR: fork failed" << std::endl;
    return;
  } else if (pid > 0) {
    // parent process
    close(pipe_out[0]); // Close reading end of the output pipe
    close(pipe_in[1]); // Close writing end of input pipe

    // we don't have to dup the stdin and stdout here, unlike in the
    // child process, because we are aware of the pipes and can
    // read-write to the directly.

    // Write input string
    // write(pipe_out[1], input_str, strlen(input_str)+1);

    // Read string from child
    // read(pipe_in[0], concat_str, 100);

  } else {
    // pid == 0, child process

    // redirect stdin and stdout to the pipes. explicitly close stdin
    // and stdout here, and also use dup2 instead of dup so that not
    // to use magic numbers for the file descriptors.
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    dup2(pipe_out[1], STDIN_FILENO);
    dup2(pipe_in[0], STDOUT_FILENO);

    int argc = args.size();
    // the arguments list must be NULL-terminated. The first arg
    // (arg0) by convention should contain the name of the program to
    // be executed. therefore we've got two more elements in the args
    // array for exec
    char* argv[argc+2];

    argv[0] = strdup(prog.c_str());

    auto iter = args.begin();
    for (int i = 1; i <= argc; ++i) {
      argv[i] = strdup((*iter).c_str());
      ++iter;
    }
    argv[argc+1] = NULL;

    // Execute the video player program
    if (execvp(prog.c_str(), argv) < 0) {
      std::cerr << "failed to execute player program" << std::endl;
    }
  }
}

VideoPlayerProxy::~VideoPlayerProxy() {
  close(pipe_out[1]);
  close(pipe_in[0]);
}
