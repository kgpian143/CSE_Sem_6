#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

char *command = NULL;

int up_arrow_handler(int count, int key) {
  if (count == 0) {
    return 0;
  }
  // Go to the previous command in history
  HIST_ENTRY *entry = previous_history();
  if (entry) {
    command = strdup(entry->line);
    rl_replace_line(command, 0);
    rl_point = rl_end;
  }
  return 1 ;
}

int down_arrow_handler(int count, int key) {
  if (count == 0) {
    return 0 ;
  }
  // Go to the next command in history
  HIST_ENTRY *entry = next_history();
  if (entry) {
    command = strdup(entry->line);
    rl_replace_line(command, 0);
    rl_point = rl_end;
  }
  return 1;
}

int main() {
  // Bind the up arrow key to the up_arrow_handler function
  rl_bind_keyseq("\033\[A", up_arrow_handler);
  // Bind the down arrow key to the down_arrow_handler function
  rl_bind_keyseq("\033\[B", down_arrow_handler);

  while ((command = readline("Enter command: ")) != NULL) {
    // Add the command to the history
    add_history(command);
    // Do something with the command
    printf("You entered: %s\n", command);
    // Free the command string
    free(command);
  }

  return 0;
}
