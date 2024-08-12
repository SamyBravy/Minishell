#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>  // compilare con -lreadline
#include <readline/history.h>

int main() {
    char *input;
    const char *prompt = "Inserisci un comando: ";

    // Loop per leggere comandi dall'utente
    while ((input = readline(prompt)) != NULL) {
        // Se l'input non è vuoto, aggiungilo alla cronologia
        if (*input) {
            add_history(input);
        }

        // Comando speciale per cancellare la cronologia
        if (strcmp(input, "clear_history") == 0) {
            rl_clear_history();
            printf("Cronologia cancellata.\n");
        } else {
            // Stampa il comando ricevuto
            printf("Hai inserito: %s\n", input);
        }

        // Libera la memoria allocata per l'input
        free(input);
    }

    return 0;
}
