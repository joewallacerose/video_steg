#include "wave.h"

FILE *ptr;
char *filename;

int main(int argc, char **argv){
    printf("Running main\n");

    filename = (char*) malloc(sizeof(char) * 1024);
    if (filename == NULL) {
        printf("Error in mallocn\n");
        exit(1);
    }

    // get file path
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {

        strcpy(filename, cwd);

        // get filename from command line
        if (argc < 2) {
            printf("No wave file specified\n");
            return 0;
        }

        strcat(filename, "/");
        strcat(filename, argv[1]);
        printf("%sn", filename);
    }

    // open file
    printf("Opening  file..\n");
    ptr = fopen(filename, "rb");
    if (ptr == NULL) {
        printf("Error opening file\n");
        exit(1);
    }

    return EXIT_SUCCESS;
}