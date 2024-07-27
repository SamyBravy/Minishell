#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>  // compilare con -lreadline
#include <readline/history.h>

int main() {
    char *input;
    const char *prompt = "Inserisci un comando: ";

    while ((input = readline(prompt)) != NULL) {
        // Aggiungi il comando alla cronologia se non è vuoto
        if (*input) {
            add_history(input);
        }

        // Stampa il comando ricevuto
        printf("Hai inserito: %s\n", input);

        // Comando speciale per uscire dal loop
        if (strcmp(input, "exit") == 0) {
            free(input);
            break;
        }

        // Libera la memoria allocata per l'input
        free(input);
    }

    return 0;
}
