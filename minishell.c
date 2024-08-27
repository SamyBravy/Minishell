#include "minishell.h"

t_input *new_input_node(t_type type, char *str)
{
    t_input *new_node;
    
    new_node = (t_input *)malloc(sizeof(t_input));
    if (!new_node)
        return NULL;
    new_node->type = type;
    if (str)
        new_node->str = strdup(str);
    else
        new_node->str = NULL;
    new_node->fd = -1;
    new_node->next = NULL;
    return new_node;
}

void add_input_node(t_input **head, t_input *new_node)
{
    t_input *temp = *head;
    t_input *last_cmd = NULL;

    if (!(*head))
    {
        *head = new_node;
        return;
    }
    while (temp)
    {
        if (temp->type == PIPE)
            last_cmd = NULL;

        if (temp->type == CMD)
            last_cmd = temp;

        if (!temp->next)
            break;

        temp = temp->next;
    }
    if (new_node->type == CMD && last_cmd)
    {
        char *temp_str = malloc(strlen(last_cmd->str) + strlen(new_node->str) + 2);
        strcpy(temp_str, last_cmd->str);
        strcat(temp_str, "\x1D");
        strcat(temp_str, new_node->str);
        free(last_cmd->str);
        last_cmd->str = temp_str;
        free(new_node->str);
        free(new_node);
    }
    else
    {
        temp->next = new_node;
    }
}


t_type identify_type(char *token)
{
    if (ft_strncmp(token, ">>", 2) == 0)
		return (APPEND);
	if (strncmp(token, "<<", 2) == 0)
		return (HEREDOC);
	if (ft_strncmp(token, ">", 1) == 0)
		return (TRUNC);
	if (strncmp(token, "<", 1) == 0)
		return (INPUT);
	if (strncmp(token, "|", 1) == 0)
		return (PIPE);
	return (CMD);
}

int spaces(char c)
{
    if ((c == 32 || c == 13 || c == 9 || c == 11))
        return 1;
    return 0;
}

t_input *tokenize(char *str)
{
    t_input *head = NULL;
    char *token;
    t_type current_type = CMD;
    char *cmd_str = NULL;
    char *temp_cmd_str;
    char temp[3];
    int i;
    char *start;

    while (*str)
    {
        while (spaces(*str) == 1)
            str++;
        if (*str == '\0')
            break;
        if (*str == '>' || *str == '<' || *str == '|')
        {
            i = 0;
            while (i++ < 3  )
                temp[i - 1] = 0; //usare bzero
            temp[0] = *str++;
            if ((*str == '>' || *str == '<') && temp[0] == *str)
                temp[1] = *str++;
            current_type = identify_type(temp);
            if (current_type == PIPE)
            {
                if (cmd_str)
                {
                    add_input_node(&head, new_input_node(CMD, cmd_str));
                    free(cmd_str);
                    cmd_str = NULL;
                }
                add_input_node(&head, new_input_node(current_type, NULL));
            }
        }
        else
        {
            start = str;
            while (*str && spaces(*str) == 0 && *str != '>' && *str != '<' && *str != '|')
                str++;
            token = strndup(start, str - start);
            if (current_type == CMD || current_type == PIPE)
            {
                if (cmd_str)
                {
                    temp_cmd_str = malloc(strlen(cmd_str) + strlen(token) + 2);
                    strcpy(temp_cmd_str, cmd_str);
                    strcat(temp_cmd_str, "\x1D");
                    strcat(temp_cmd_str, token);
                    free(cmd_str);
                    cmd_str = temp_cmd_str;
                }
                else
                    cmd_str = strdup(token);
            }
            else
            {
                if (cmd_str)
                {
                    add_input_node(&head, new_input_node(CMD, cmd_str));
                    free(cmd_str);
                    cmd_str = NULL;
                }
                add_input_node(&head, new_input_node(current_type, token));
                current_type = CMD;
            }
            free(token);
        }
    }

    if (cmd_str)
    {
        add_input_node(&head, new_input_node(CMD, cmd_str));
        free(cmd_str);
    }

    return head;
}

void print_tokens(t_input *tokens)
{
    while (tokens)
    {
        printf("Type: %d, Str: %s, FD: %d\n", tokens->type, tokens->str, tokens->fd);
        tokens = tokens->next;
    }
}

int is_quote_balanced(const char *str)
{
    int in_double_quote = -1;
    int in_single_quote = -1;

    while (*str)
    {
        if (*str == '"')
        {
            if (in_single_quote == -1)
                in_double_quote = -1 * in_double_quote;
        }
        else if (*str == '\'')
        {
            if (in_double_quote == -1)
                in_single_quote = -1 * in_single_quote;
        }
        str++;
    }

    if (in_double_quote == -1 && in_single_quote == -1)
        return 1;
    return -1;
}

int main(int argc, char **argv, char **env)
{
    char *input;
    char *temp_input;
    int exit_status;
    t_input *tokens;
    t_list *lst_env;

    (void)argc;
    (void)argv;
    read_history(".tmp/.history.txt");
    lst_env = ft_matrix_to_lst(env);
    if (ft_getenv("OLDPWD", lst_env) == NULL)
        ft_export("OLDPWD", &lst_env);
    int tmp = dup(STDIN_FILENO); // per avere tutto perfettamente pulito
	dup2(tmp, STDIN_FILENO); // per avere tutto perfettamente pulito
	close(tmp); // per avere tutto perfettamente pulito
    tmp = dup(STDOUT_FILENO); // per avere tutto perfettamente pulito
    dup2(tmp, STDOUT_FILENO); // per avere tutto perfettamente pulito
    close(tmp); // per avere tutto perfettamente pulito
    while (true)
    {
        input = readline("minicecco> ");
        while (is_quote_balanced(input) == -1)
        {
            temp_input = readline("> ");
            char *new_input = malloc(strlen(input) + strlen(temp_input) + 2);
            if (!new_input)
                return 1;
            strcpy(new_input, input);
            strcat(new_input, "\n");
            strcat(new_input, temp_input);
            free(input);
            input = new_input;
            free(temp_input);
        }
		if (input && *input) // Assicurati che input non sia NULL e non sia una stringa vuota
        {
            add_history(input);
            append_history(1, ".tmp/.history.txt");
            if (check_syntax_errors(input) != 0)
            {
                exit_status = 2;
                free(input);
                continue; // Evita di terminare il programma, passa alla prossima iterazione
            }
            tokens = tokenize(input);
            free(input);
            executer(&tokens, &lst_env, &exit_status);
        }
    }
    close(STDIN_FILENO); // per avere tutto perfettamente pulito
    close(STDOUT_FILENO); // per avere tutto perfettamente pulito
	ft_lstclear(&lst_env, free);
    return 0;
}


/*int main()
{
    char *input;
    t_input *tokens;
    
    // Carica la cronologia da un file, se esiste
    read_history("history.txt");
    
    while (1)
    {
        input = readline("miniCecco >");
        if (!input)
            break;
        if (*input)
        {
            add_history(input);
            append_history(1, "history.txt");
        }
        tokens = tokenize(input);
        print_tokens(tokens);
        t_input *free_tmp;
        while (tokens)
        {
            free_tmp = tokens;
            tokens = tokens->next;
            free(free_tmp->str);
            free(free_tmp);
        }
        free(input);
    }    
    return 0;
}*/
   
   
   
/*int main()
{ 
    char *input = "echo ciao > file.txt echo casa";
    t_input *tokens = tokenize(input);
    print_tokens(tokens);
    t_input *free_tmp;
    while (tokens)
    {
        free_tmp = tokens;
        tokens = tokens->next;
        free(free_tmp->str);
        free(free_tmp);
    }
    return 0;
}*/
