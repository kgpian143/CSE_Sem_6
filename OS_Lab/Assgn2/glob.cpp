#include <glob.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

int main() {
  glob_t results;
  char pattern[] = "d*/f*.txt";

  int ret = glob(pattern, 0, NULL, &results);
  if (ret == GLOB_NOMATCH) {
    printf("No matching files found\n");
  } else if (ret != 0) {
    printf("Error while expanding wildcard pattern: %s\n", strerror(errno));
  } else {
    for (size_t i = 0; i < results.gl_pathc; i++) {
      printf("%s\n", results.gl_pathv[i]);
    }
  }

  globfree(&results);
  return 0;
}
