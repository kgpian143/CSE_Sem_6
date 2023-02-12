#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <readline/readline.h>

volatile sig_atomic_t stop;
char *command = NULL;

void int_handler(int sig) {
  stop = 1;
}

void tstp_handler(int sig) {
  signal(SIGTSTP, SIG_DFL);
  rl_free_line_state();
  rl_cleanup_after_signal();
  kill(getpid(), SIGTSTP);
}

int main() {
  signal(SIGINT, int_handler);
  signal(SIGTSTP, tstp_handler);
  while (!stop) {
    command = readline("Enter command: ");
    if (command) {
      // Do something with the command
      printf("You entered: %s\n", command);
      // Free the command string
      free(command);
    }
  }
  return 0;
}
