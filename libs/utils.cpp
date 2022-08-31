#include "utils.h"

FILE *openFile(char *path) {
    FILE *file;
    file = fopen(path, "r");

    if (file == nullptr) {
        printf("Erro ao tentar abrir o arquivo!");
        exit(1);
    }

    return file;
}