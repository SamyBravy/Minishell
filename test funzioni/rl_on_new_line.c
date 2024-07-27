#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>  // compilare con -lreadline
#include <readline/history.h>

void print_message(const char *message) {
    // Stampa un messaggio a schermo
    printf("%s\n", message);
    
    // Notifica a Readline che siamo su una nuova riga
    rl_on_new_line();
    // Riscrive il prompt e l'input corrente
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

        // Comando speciale per stampare un messaggio
        if (strcmp(input, "show_message") == 0) {
            print_message("Questo è un messaggio!");
        } else {
            // Stampa il comando ricevuto
            printf("Hai inserito: %s\n", input);
        }

        // Libera la memoria allocata per l'input
        free(input);
    }

    return 0;
}
