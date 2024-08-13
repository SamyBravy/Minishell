#include <stdio.h>
#include <unistd.h>

void check_access(const char *filepath) {
    // Verifica se il file esiste
    if (access(filepath, F_OK) == 0) {
        printf("Il file '%s' esiste.\n", filepath);
    } else {
        printf("Il file '%s' non esiste.\n", filepath);
        return;
    }

    // Verifica se il file è leggibile
    if (access(filepath, R_OK) == 0) {
        printf("Il file '%s' è leggibile.\n", filepath);
    } else {
        printf("Il file '%s' non è leggibile.\n", filepath);
    }

    // Verifica se il file è scrivibile
    if (access(filepath, W_OK) == 0) {
        printf("Il file '%s' è scrivibile.\n", filepath);
    } else {
        printf("Il file '%s' non è scrivibile.\n", filepath);
    }

    // Verifica se il file è eseguibile
    if (access(filepath, X_OK) == 0) {
        printf("Il file '%s' è eseguibile.\n", filepath);
    } else {
        printf("Il file '%s' non è eseguibile.\n", filepath);
    }
}

int main() {
    const char *filepath = "testfile.txt";
    check_access(filepath);
    return 0;
}
