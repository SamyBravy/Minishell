#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>  // compilare con -lreadline
#include <readline/history.h>

void replace_input_with(const char *new_input) {
    // Sostituisce il contenuto corrente della riga di input con new_input
    rl_replace_line(new_input, 1);
    // Aggiorna il display per riflettere il nuovo contenuto
    rl_redisplay();
}

int main() {
    char *input;
    const char *prompt = "Inserisci un comando: ";

    while ((input = readline(prompt)) != NULL) {
        // Aggiungi il comando alla cronologia se non è vuoto
        if (*input) {
            add_history(input);
        }

        // Comando speciale per sostituire l'input corrente
        if (strcmp(input, "replace_input") == 0) {
            replace_input_with("Questo è il nuovo input");
        } else {
            // Stampa il comando ricevuto
            printf("Hai inserito: %s\n", input);
        }

        // Libera la memoria allocata per l'input
        free(input);
    }

    return 0;
}
