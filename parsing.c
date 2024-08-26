#include "minishell.h"


t_type which_token(char *token)
{
    if (ft_strncmp(token, ">", 1) == 0)
        return TRUNC;
    if (ft_strncmp(token, ">>", 2) == 0)
        return APPEND;
    if (strncmp(token, "<", 1) == 0)
        return INPUT;
    if (strncmp(token, "<<", 2) == 0)
        return HEREDOC;
    if (strncmp(token, "|", 1) == 0)
        return PIPE;
    return CMD;
}

int syntax_error(char *error_token)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `", STDERR_FILENO);
	ft_putstr_fd(error_token, STDERR_FILENO);
	ft_putstr_fd("'\n", STDERR_FILENO);
	return -1;
}

int jump_space(char **str)
{
	char *temp_str;

	temp_str = *str;
	while (**str && **str == ' ')
		(*str)++;
	return (*str != temp_str);
}

int jump_quotes(char **str)
{
	char quote;
	char *temp_str;

	temp_str = *str;
	if (**str == '\'' || **str == '\"')
	{
		quote = **str;
		(*str)++;
		while (**str && **str != quote)
			(*str)++;
		if (**str)
			(*str)++;
	}
	return (*str != temp_str);
}

void	jump_space_and_quotes(char **str)
{
	while (jump_space(str) || jump_quotes(str))
		;
}
int check_syntax_errors(char *token)
{
	jump_space(&token);
	if(which_token(token) == PIPE)
		return syntax_error("|");
	jump_space_and_quotes(&token);
	while(*token)
	{
		if(which_token(token) == PIPE)
		{
			token++;
			jump_space(&token);
			if(which_token(token) == PIPE)
				return syntax_error("|");
		}
		else if(which_token(token) == HEREDOC || which_token(token) == APPEND
			|| which_token(token) == INPUT || which_token(token) == TRUNC)
		{
			token++;
			if (which_token(token) == HEREDOC || which_token(token) == APPEND)
				token++;
			jump_space(&token);
			if (which_token(token) == PIPE)
				return syntax_error("|");
			if (which_token(token) == HEREDOC)
				return syntax_error("<<");
			if (which_token(token) == APPEND)
				return syntax_error(">>");
			if (which_token(token) == INPUT)
				return syntax_error("<");
			if (which_token(token) == TRUNC)
				return syntax_error(">");
			if (which_token(token) == CMD)
				return syntax_error("newline");
		}
		else
			jump_space_and_quotes(&token);
		ft_printf("token: %s\n", token);
	}
	return 0; // Nessun errore di sintassi rilevato
}
