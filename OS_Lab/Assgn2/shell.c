#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define BUFSIZE 100

int main(void) {
  char cmd[BUFSIZE];

  while (1) {
    printf("$ ");
    if (fgets(cmd, BUFSIZE, stdin) == NULL) break;
    int len = strlen(cmd);
    if (cmd[len - 1] == '\n') cmd[len - 1] = '\0';

    pid_t pid = fork();
    if (pid == -1) {
      perror("fork");
      exit(1);
    }

    if (pid == 0) {
      execlp(cmd, cmd, (char *)NULL);
      perror(cmd);
      exit(1);
    }

    int status;
    if (waitpid(pid, &status, 0) == -1) {
      perror("waitpid");
      exit(1);
    }
  }

  return 0;
}
