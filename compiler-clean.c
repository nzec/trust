#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s input_file output_file\n", argv[0]);
        return 1;
    }
    FILE *in_file = NULL;
    if ((in_file = fopen(argv[1], "r")) == NULL) {
        fprintf(stderr, "%s: No such file or directory\n", argv[1]);
        return 1;
    }

    fseek(in_file, 0, SEEK_END);
    int in_size = ftell(in_file);
    rewind(in_file);

    char *buffer = malloc(in_size + 1);
    buffer[in_size] = '\0';

    if (fread(buffer, 1, in_size, in_file) != in_size) {
        fprintf(stderr, "%s: copying to buffer failed\n", argv[1]);
        return 1;
    }
    
    fclose(in_file);

    char cc[500];
    snprintf(cc, 500, "gcc -o %s -xc -", argv[2]);

    FILE * gcc = NULL;
    if ((gcc = popen(cc, "w")) == NULL) {
        fprintf(stderr, "%s: pipe failed\n", argv[1]);
        return 1;
    }

    fwrite(buffer, sizeof(char), in_size, gcc);
    pclose(gcc);
    free(buffer);
}
