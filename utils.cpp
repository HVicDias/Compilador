#include "utils.h"

FILE *openFile(char *path) {
    FILE *f;
    f = fopen(path, "r");

    if (f == NULL) {
        printf("Erro ao tentar abrir o arquivo!");
        exit(1);
    }

    return f;
}