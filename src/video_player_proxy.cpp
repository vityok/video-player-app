#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#include <iostream>
#include <list>
#include <string>
#include <thread>

#include "video-player-app.hpp"

using std::string;

void VideoPlayerProxy::spawn_process(const string& prog, std::list<string>& args) {

  // quick note from the pipe(2) man page:
  //
  // pipefd[0] refers to the read end of the pipe.
  // pipefd[1] refers to the write end of the pipe.
  if (pipe(pipe_out)==-1) {
    std::cerr << "ERROR: pipe failed" << std::endl;
    return ;
  }
  if (pipe(pipe_in)==-1) {
    std::cerr << "ERROR: pipe failed" << std::endl;
    return ;
  }

  child_pid = fork();
  if (child_pid < 0) {
    std::cerr << "ERROR: fork failed" << std::endl;
    return;
  } else if (child_pid > 0) {
    // parent process
    std::cout<< "spawned process: " << prog << " pid=" << child_pid << std::endl;

    close(pipe_out[0]); // Close reading end of the output pipe
    close(pipe_in[1]); // Close writing end of input pipe

    // launch background thread waiting for the child process to exit
    // (change state)
    monitor_thread.reset(new std::thread(&VideoPlayerProxy::wait_child, this));
  } else {
    // child_pid == 0, child process
    close(pipe_out[1]); // writing end of the parent output pipe
    close(pipe_in[0]); // reading end of parent input pipe

    // redirect stdin and stdout to the pipes. use dup2 instead of dup
    // so that not to use magic numbers for the file descriptors.
    dup2(pipe_out[0], STDIN_FILENO);
    dup2(pipe_in[1], STDOUT_FILENO);

    int argc = args.size();

    // the arguments list must be NULL-terminated. The first arg
    // (arg0) by convention should contain the name of the program to
    // be executed. therefore we've got two more elements in the args
    // array for exec than the number of given parameters
    char** argv = (char**) calloc (argc+2, sizeof(char*));

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
    // at this moment child process should be replaced by the
    // specified executable
  }
}


void VideoPlayerProxy::pipe_msg(const string& msg)
{
  ssize_t msg_size = msg.length();
  int sent = write(pipe_out[1], msg.c_str(), msg_size);
  if (sent < 0) {
    std::cerr << "VideoPlayerProxy::pipe_msg ERROR sent " << sent
	      << " out of " << msg_size << std::endl;
  } else if (sent == 0) {
    std::cerr << "VideoPlayerProxy::pipe_msg nothing sent out of "
	      << msg_size << std::endl;
  } else if (sent < msg_size) {
    std::cerr << "VideoPlayerProxy::pipe_msg sent " << sent
	      << " out of " << msg_size << std::endl;
  }
}


void VideoPlayerProxy::wait_child()
{
  std::cout << "waiting for child to exit" << std::endl;
  int status;
  do {
    int w = waitpid(child_pid, &status, WUNTRACED | WCONTINUED);
    if (w == -1) {
      std::cerr << "waitpid" << std::endl;
      exit(EXIT_FAILURE);
    }

    if (WIFEXITED(status)) {
      const int exit_status = WEXITSTATUS(status);
      printf("exited, status=%d\n", exit_status);
      for (AVideoPlayerStatusListener& listener : _video_player_status_listeners) {
	listener.handle_exited(exit_status);
      }
    } else if (WIFSIGNALED(status)) {
      printf("killed by signal %d\n", WTERMSIG(status));
    } else if (WIFSTOPPED(status)) {
      printf("stopped by signal %d\n", WSTOPSIG(status));
    } else if (WIFCONTINUED(status)) {
      printf("continued\n");
    }
  } while (!WIFEXITED(status) && !WIFSIGNALED(status));
}

void VideoPlayerProxy::stop()
{
  close(pipe_out[1]);
  close(pipe_in[0]);
  kill(child_pid, SIGKILL);
  std::cout << "VideoPlayerProxy::stop done" << std::endl;
}


void VideoPlayerProxy::add_video_player_status_listener(AVideoPlayerStatusListener& listener)
{
  _video_player_status_listeners.push_front(listener);
}

VideoPlayerProxy::~VideoPlayerProxy() {
  std::cout << "~VideoPlayerProxy: done" << std::endl;
}
