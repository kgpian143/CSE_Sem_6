#include <iostream>
#include <unistd.h>
#include <cstring>
#include <vector>

int main() {
  int n = 3;
  std::vector<std::vector<char*>> commands = {
    { (char*)"ls", NULL },
    { (char*)"grep", (char*)"shell", NULL },
    { (char*)"sort", NULL },
  };

  int pipefd[2];
  pid_t pid;

  for (int i = 0; i < n - 1; i++) {
    if (pipe(pipefd) == -1) {
      std::cerr << "Error creating pipe" << std::endl;
      return 1;
    }

    pid = fork();
    if (pid == -1) {
      std::cerr << "Error forking process" << std::endl;
      return 1;
    }

    if (pid == 0) {
      close(pipefd[0]);
      dup2(pipefd[1], STDOUT_FILENO);
      close(pipefd[1]);

      execvp(commands[i][0], commands[i].data());

      std::cerr << "Error executing command" << std::endl;
      return 1;
    } else {
      close(pipefd[1]);
      dup2(pipefd[0], STDIN_FILENO);
      close(pipefd[0]);
    }
  }

  execvp(commands[n - 1][0], commands[n - 1].data());

  std::cerr << "Error executing command" << std::endl;
  return 1;
}
