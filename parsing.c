#include "minishell.h"


bool is_redirection(char *token)
{
    return (ft_strcmp(token, ">") == 0 || ft_strcmp(token, "<") == 0 ||
            ft_strcmp(token, ">>") == 0 || ft_strcmp(token, "<<") == 0);
}

bool is_pipe(char *token)
{
    return (ft_strcmp(token, "|") == 0 || ft_strcmp(token, "||") == 0);
}

bool is_special_char(char *token)
{
    return (is_redirection(token) || is_pipe(token));
}

bool is_valid_command(char *token)
{
    // A valid command should not be a special character, and should start with a letter or a valid symbol.
    return (!is_special_char(token) && (isalpha(token[0]) || token[0] == '.' || token[0] == '/'));
}

int syntax_error(char *error_token)
{
    printf("minicecco: syntax error near unexpected token '%s'\n", error_token);
    return -1;
}

int check_syntax_errors(char *token)
{
    bool expecting_command = true;
    bool quote_open = false;
    char quote_type = '\0';

    // Gestione delle virgolette aperte
    if (token[0] == '\'' || token[0] == '"')
    {
        if (quote_open && quote_type == token[0])
        {
                            quote_open = false;
            }
            else if (!quote_open)
            {
                quote_open = true;
                quote_type = token[0];
            }
        }

        // Controllo dei pipe
        if (is_pipe(token))
        {
            if (expecting_command || !token || is_pipe(token))
            {
                return syntax_error(token); // Segnala il pipe errato
            }
            expecting_command = true; // Dopo un pipe, ci si aspetta un comando
        }
        // Controllo delle redirezioni
        else if (is_redirection(token))
        {
            if (expecting_command || !token || is_special_char(token))
            {
                return syntax_error(token); // Segnala la redirezione errata
            }
            expecting_command = true; // Dopo una redirezione, ci si aspetta un file
        }
        // Comandi non validi
        else if (!is_valid_command(token))
        {
            return syntax_error(token); // Segnala il comando o il token non valido
        }
        else
        {
            expecting_command = false; // Comando valido trovato
        }


    // Controllo delle virgolette non chiuse
    if (quote_open)
    {
        return syntax_error("newline"); // Segnala "newline" per virgolette non chiuse
    }

    // Se ci si aspetta ancora un comando, c'è un problema di sintassi
    if (expecting_command)
    {
        return syntax_error("newline"); // Segnala "newline" se manca un comando alla fine
    }

    return 0; // Nessun errore di sintassi rilevato
}





/*int check_syntax_errors(char **tokens)
{
    int i = 0;
    bool expecting_command = true;
    bool quote_open = false;
    char quote_type = '\0';

    while (tokens[i])
    {
        char *token = tokens[i];

        // Handle unclosed quotes
        if (token[0] == '\'' || token[0] == '"')
        {
            if (quote_open && quote_type == token[0])
            {
                quote_open = false;
            }
            else if (!quote_open)
            {
                quote_open = true;
                quote_type = token[0];
            }
        }

        if (is_pipe(token))
        {
            if (expecting_command || !tokens[i + 1] || is_pipe(tokens[i + 1]))
            {
                return syntax_error(token);
            }
            expecting_command = true; // After a pipe, we expect a command
        }
        else if (is_redirection(token))
        {
            if (expecting_command || !tokens[i + 1] || is_special_char(tokens[i + 1]))
            {
                return syntax_error(token);
            }
            expecting_command = true; // After a redirection, we expect a filename
        } else if (!is_valid_command(token))
        {
            return syntax_error(token);
        }
        else
        {
            expecting_command = false; // Valid command found
        }

        i++;
    }

    // Check if there's an unclosed quote at the end
    if (quote_open)
    {
        return syntax_error("newline");
    }

    // If we're still expecting a command, there's a syntax issue
    if (expecting_command)
    {
        return syntax_error("newline");
    }

    return 0; // No syntax errors detected
}*/

/*int main() {
    // Example usage: replace this with your tokenized input
    char *example_input[] = {"echo", ">", "file.txt", "|", "cat", NULL};

    // Check for syntax errors
    int result = check_syntax_errors(example_input);

    if (result == 0) {
        printf("No syntax errors detected.\n");
        // Proceed with execution
    } else {
        // Handle the error appropriately
    }

    return result;
}*/