#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

#define MAX_COMMAND_LENGTH 100
#define MAX_PID_LENGTH 10

// Function to read the input from user
int read_input(char *input, int size) {
  int i = 0;
  int c;

  while (i < size - 1 && (c = getchar()) != '\n' && c != EOF) {
    input[i++] = c;
  }

  input[i] = '\0';

  return i;
}

// Function to get the list of process IDs that are holding a lock on the file
void get_process_pids(const char *filepath, char pids[][MAX_PID_LENGTH], int *count) {
  char command[MAX_COMMAND_LENGTH];
  sprintf(command, "lsof %s | awk 'NR>1 {print $2}'", filepath);

  FILE *fp = popen(command, "r");
  if (fp == NULL) {
    perror("Failed to get process IDs");
    exit(1);
  }

  *count = 0;
  while (fgets(pids[*count], MAX_PID_LENGTH, fp) != NULL) {
    pids[*count][strlen(pids[*count]) - 1] = '\0';
    (*count)++;
  }

  pclose(fp);
}

// Function to delete the file after killing the processes
void delete_file(const char *filepath) {
  char command[MAX_COMMAND_LENGTH];
  sprintf(command, "rm %s", filepath);

  int status = system(command);
  if (status == -1) {
    perror("Failed to delete file");
    exit(1);
  }
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("Usage: %s <filepath>\n", argv[0]);
    exit(1);
  }

  const char *filepath = argv[1];
  char pids[100][MAX_PID_LENGTH];
  int count;

  get_process_pids(filepath, pids, &count);
  if (count == 0) {
    printf("No process is holding a lock on the file.\n");
    return 0;
  }

  printf("Following processes are holding a lock on the file:\n");
  for (int i = 0; i < count; i++) {
    printf("%s\n", pids[i]);
  }

  printf("Do you want to kill these processes and delete the file (y/n)? ");
  char input[10];
  read_input(input, 10);

  if (strcmp(input, "y\n") == 0) {
    for (int i = 
