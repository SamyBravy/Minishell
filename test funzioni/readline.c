#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>  // compilare con -lreadline
#include <readline/history.h>

int main() {
    // Prompt per l'input dell'utente
    const char *prompt = "Inserisci una linea di testo: ";
    
    // Legge una linea di input dall'utente
    char *input = readline(prompt);
    
    // Controlla se l'input è valido (non NULL)
    if (input) {
        // Stampa l'input ricevuto
        printf("Hai inserito: %s\n", input);
        
        // Aggiunge l'input alla cronologia
        add_history(input);
        
        // Libera la memoria allocata per l'input
        free(input);
    } else {
        printf("Errore nella lettura dell'input o input vuoto.\n");
    }

    return 0;
}
