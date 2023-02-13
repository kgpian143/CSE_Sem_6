#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    char *file_path = argv[1];

    if (access(file_path, R_OK) == 0) {
        printf("The file is readable.\n");
    } else {
        printf("The file is not readable.\n");
    }

    if (access(file_path, W_OK) == 0) {
        printf("The file is writable.\n");
    } else {
        printf("The file is not writable.\n");
    }

    return 0;
}
