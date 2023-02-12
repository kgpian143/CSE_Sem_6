#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LEN 256

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("Usage: %s <pid>\n", argv[0]);
    return 1;
  }

  int pid = atoi(argv[1]);
  char stat_file_path[32];
  sprintf(stat_file_path, "/proc/%d/stat", pid);

  FILE *file = fopen(stat_file_path, "r");
  if (!file) {
    printf("Could not open file %s\n", stat_file_path);
    return 1;
  }

  char *line ;
  line = new char[MAX_LINE_LEN] ;
  fgets(line, MAX_LINE_LEN, file);
  fclose(file);

  char *token;
  int i = 0;
  while ((token = strsep( &line , " ")) != NULL) {
    i++;
    if (i == 14) {
      long utime = atol(token);
      i++;
      token = strsep(  &line , " ");
      long stime = atol(token);
      printf("Accumulated CPU time: %ld\n", utime + stime);
      break;
    }
  }

  return 0;
}
