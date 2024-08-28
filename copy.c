#include "minishell.h"
#include <ctype.h>
#include <fcntl.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char	*ft_strndup(const char *s, size_t n)
{
	char	*src;
	char	*copy;
	size_t	i;

	src = (char *)s;
	i = 0;
	if (n < ft_strlen(s))
		copy = malloc(sizeof(char) * (n + 1));
	else
		copy = malloc(sizeof(char) * (ft_strlen(s) + 1));
	if (!copy)
		return (NULL);
	while (src[i] && i < n)
	{
		copy[i] = src[i];
		i++;
	}
	copy[i] = '\0';
	return (copy);
}

size_t	calculate_length_samu(const char *str, t_list *env)
{
	t_vars_samu	vars;

	vars.i = 0;
	vars.len = ft_strlen(str);
	vars.result_len = 0;
	vars.env = env;
	while (vars.i < vars.len)
	{
		if (str[vars.i] == '$')
		{
			vars.var_start = vars.i + 1;
			vars.var_len = 0;
			while (vars.var_start + vars.var_len < vars.len
				&& ((ft_isalnum(str[vars.var_start + vars.var_len])
					|| str[vars.var_start + vars.var_len] == '?')
					|| str[vars.var_start + vars.var_len] == '_'))
			{
				vars.var_len++;
			}
			if (vars.var_len > 0)
			{
				vars.var_name = ft_strndup(str + vars.var_start, vars.var_len);
				vars.var_value = ft_getenv(vars.var_name, vars.env);
				free(vars.var_name);
				if (vars.var_value)
				{
					vars.result_len += strlen(vars.var_value);
				}
				vars.i += vars.var_len;
			}
			else
			{
				vars.result_len++;
			}
		}
		else
		{
			vars.result_len++;
		}
		vars.i++;
	}
	return (vars.result_len);
}

void	copy_value(t_vars_samu *vars, char *result)
{
	size_t	value_len;
	size_t		i;

	value_len = ft_strlen(vars->var_value);
	i = 0;
	while (i < value_len)
		result[vars->result_len++] = vars->var_value[i++];
}

void	expand(t_vars_samu *vars, const char *str, char *result)
{
	vars->var_name = ft_strndup(str + vars->var_start, vars->var_len);
	vars->var_value =ft_getenv(vars->var_name, vars->env);
	free(vars->var_name);
	if (vars->var_value)
		copy_value(vars, result);
}

void	check_dollar(t_vars_samu *vars, const char *str, char *result)
{
	if (str[vars->i] == '$')
	{
		vars->var_start = vars->i + 1;
		vars->var_len = 0;
		while (vars->var_start + vars->var_len < vars->len
			&& ((ft_isalnum(str[vars->var_start + vars->var_len])
				|| str[vars->var_start + vars->var_len] == '?')
				|| str[vars->var_start + vars->var_len] == '_'))
		{
			vars->var_len++;
		}
		if (vars->var_len > 0)
		{
			expand(vars, str, result);
			vars->i += vars->var_len;
		}
		else
		{
			result[vars->result_len++] = str[vars->i];
		}
	}
	else
	{
		result[vars->result_len++] = str[vars->i];
	}
}

t_vars_samu	*init_expand_samu(const char *str)
{
	t_vars_samu	*vars;

	vars = malloc(sizeof(t_vars_samu));
	vars->i = 0;
	vars->len = ft_strlen(str);
	vars->result_len = 0;
	return (vars);
}

char	*expand_samu(const char *str, t_list *env)
{
	t_vars_samu *vars;
	size_t expanded_length;
	char *result;

	vars = init_expand_samu(str);
	vars->env = env;
	expanded_length = calculate_length_samu(str, env);
	result = malloc(expanded_length + 1);
	while (vars->i < vars->len)
	{
		check_dollar(vars, str, result);
		vars->i++;
	}
	result[vars->result_len] = '\0';
	free(vars);
	return(result);
}

t_input	*new_node(t_type type, char *str)
{
	t_input	*new_node;

	new_node = (t_input *)malloc(sizeof(t_input));
	new_node->type = type;
	if (str)
		new_node->str = ft_strndup(str, INT_MAX);
	else
		new_node->str = NULL;
	new_node->fd = -1;
	new_node->next = NULL;
	return (new_node);
}

void	find_lst_cmd(t_input *temp, t_input **last_cmd)
{
	t_input	*save;
	
	//printf("GG\n");
	save = temp;
	while ((temp))
	{
		if ((temp)->type == PIPE)
			(*last_cmd) = NULL;
		if ((temp)->type == CMD)
			(*last_cmd) = (temp);
		if (!(temp)->next)
			break ;
		(temp) = (temp)->next;
		//printf("GG\n");
	}
	/*if (*last_cmd)
		printf("LAST CMD: %s\n", (*last_cmd)->str);
	else
		printf("NOOO LASTCMD\n");*/
	temp = save;
}

void	concatenate_str(t_input *new_node, t_input *temp, t_input *last_cmd,
		char *temp_str)
{
	if (*(last_cmd->str) == '$')
		temp->next = new_node;
	else
	{
		temp_str = malloc(ft_strlen(last_cmd->str) + ft_strlen(new_node->str)
				+ 2);
		ft_strlcpy(temp_str, last_cmd->str, INT_MAX);
		ft_strlcat(temp_str, "\x1D", INT_MAX);
		ft_strlcat(temp_str, new_node->str, INT_MAX);
		free(last_cmd->str);
		last_cmd->str = temp_str;
		free(new_node->str);
		free(new_node);
	}
}

void	add_node(t_input **head, t_input *new_node)
{
	t_input	*temp;
	t_input	*last_cmd;
	char	*temp_str;

	temp_str = NULL;
	temp = *head;
	last_cmd = NULL;
	//printf("IN ADD NODE\n");
	if (!(*head))
	{
		*head = new_node;
		return ;
	}
	find_lst_cmd(temp, &last_cmd);
	if (new_node->type == CMD && last_cmd)
		concatenate_str(new_node, temp, last_cmd, temp_str);
	else
	{
		while(temp->next)
			temp = temp->next;
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

int	spaces(char c)
{
	if ((c == 32 || c == 13 || c == 9 || c == 11))
		return (1);
	return (0);
}
void	special_char(t_input **head, token_variables *vars)
{
	vars->i = 0;
	while (vars->i++ <= 2)
		vars->temp[vars->i - 1] = 0;
	vars->temp[0] = *vars->copy_str++;
	if ((*vars->copy_str == '>' || *vars->copy_str == '<')
		&& vars->temp[0] == *vars->copy_str)
		vars->temp[1] = *vars->copy_str++;
	vars->current_type = identify_type(vars->temp);
	if (vars->current_type == PIPE)
	{
		//printf("IF PIPE\n");
		if (vars->cmd_str)
		{
			//printf("STO PER AGGIUNGERE FIRST CMD\n");
			add_node(head, new_node(CMD, vars->cmd_str));
			free(vars->cmd_str);
			vars->cmd_str = NULL;
		}
		add_node(head, new_node(vars->current_type, NULL));
	}
}

void	quotes(token_variables *vars)
{
	vars->c_for_quotes = *vars->copy_str;
	vars->start = vars->copy_str++;
	while (*vars->copy_str && *vars->copy_str != vars->c_for_quotes)
		vars->copy_str++;
	vars->copy_str++;
	vars->token = ft_strndup(vars->start, vars->copy_str - vars->start);
}
void	normal_char(token_variables *vars)
{
	vars->start = vars->copy_str;
	while (*vars->copy_str && spaces(*vars->copy_str) == 0
		&& *vars->copy_str != '>' && *vars->copy_str != '<'
		&& *vars->copy_str != '|')
	{
		if (*vars->copy_str == '"' || *vars->copy_str == '\'')
		{
			vars->c_for_quotes = *vars->copy_str;
			vars->copy_str++;
			while (*vars->copy_str && *vars->copy_str != vars->c_for_quotes)
				vars->copy_str++;
		}
		if (*vars->copy_str)
			vars->copy_str++;
	}
	vars->token = ft_strndup(vars->start, vars->copy_str - vars->start);
	//printf("%s\n", vars->token);
}

void	save_cmd(token_variables *vars)
{
	if (vars->cmd_str)
	{
		//printf("QUIII???\n");
		vars->temp_cmd_str = calloc(ft_strlen(vars->cmd_str)
				+ ft_strlen(vars->token) + 2, 1);
		ft_strlcpy(vars->temp_cmd_str, vars->cmd_str, INT_MAX);
		ft_strlcat(vars->temp_cmd_str, "\x1D", INT_MAX);
		ft_strlcat(vars->temp_cmd_str, vars->token, INT_MAX);
		free(vars->cmd_str);
		vars->cmd_str = vars->temp_cmd_str;
	}
	else
	{
		//printf("STO PER CREARE CMDSTR\n");
		vars->cmd_str = ft_strndup(vars->token, INT_MAX);
	}
}

void	save_special(t_input **head, token_variables *vars)
{
	if (vars->cmd_str)
	{
		add_node(head, new_node(CMD, vars->cmd_str));
		free(vars->cmd_str);
		vars->cmd_str = NULL;
	}
	add_node(head, new_node(vars->current_type, vars->token));
	vars->current_type = CMD;
}

void	save_not_pipe(t_input **head, token_variables *vars)
{
	if (*vars->copy_str == '"' || *vars->copy_str == '\'')
		quotes(vars);
	else
		normal_char(vars);
	if (vars->current_type == CMD || vars->current_type == PIPE)
		save_cmd(vars);
	else
		save_special(head, vars);
	free(vars->token);
}

token_variables	*prepare_var(char *str)
{
	token_variables	*vars;

	vars = malloc(sizeof(token_variables));
	vars->token = NULL;
	vars->current_type = CMD;
	vars->cmd_str = NULL;
	vars->copy_str = str;
	return (vars);
}

t_input	*tokenize(char *str)
{
	t_input			*head;
	token_variables	*vars;

	head = NULL;
	vars = prepare_var(str);
	while (*vars->copy_str)
	{
		while (spaces(*vars->copy_str) == 1)
			vars->copy_str++;
		if (*vars->copy_str == '\0')
			break ;
		if (*vars->copy_str == '>' || *vars->copy_str == '<'
			|| *vars->copy_str == '|')
			special_char(&head, vars);
		else
			save_not_pipe(&head, vars);
		//printf("%s\n", vars->cmd_str);
	}
	if (vars->cmd_str)
	{
		add_node(&head, new_node(CMD, vars->cmd_str));
		free(vars->cmd_str);
	}
	free(vars);
	return (head);
}

void	print_tokens(t_input *tokens)
{
	while (tokens)
	{
		printf("Type: %d, Str: %s, FD: %d\n", tokens->type, tokens->str,
			tokens->fd);
		tokens = tokens->next;
	}
}

int	is_quote_balanced(const char *str)
{
	int	in_double_quote;
	int	in_single_quote;

	in_double_quote = -1;
	in_single_quote = -1;
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
		return (1);
	return (-1);
}

int	is_pipe_balanced(const char *str)
{
	const char	*ptr = str;

	while (*ptr)
	{
		if (*ptr == '|')
		{
			ptr++;
			while (*ptr && spaces(*ptr) == 1)
				ptr++;
			if (*ptr == '\0')
				return (-1);
		}
		ptr++;
	}
	return (1);
}

void	skip_spaces(const char *var_value, t_size_t_clean *vars)
{
	while (vars->i < vars->len && spaces(var_value[vars->i]) == 1)
	{
		vars->i++;
	}
}

void	spaces_check(const char *var_value, t_input *current,
		t_size_t_clean *vars, char *cleaned_value)
{
	while (vars->i < vars->len)
	{
		if (spaces(var_value[vars->i]) == 1)
		{
			if (current->type != CMD)
				current->fd = -42;
			cleaned_value[vars->cleaned_len++] = '\x1D';
			skip_spaces(var_value, vars);
		}
		else
		{
			cleaned_value[vars->cleaned_len++] = var_value[vars->i++];
		}
	}
}

char	*clean_variable_value(const char *var_value, t_input *current)
{
	t_size_t_clean	vars;
	char			*cleaned_value;

	vars.len = ft_strlen(var_value);
	cleaned_value = malloc(vars.len + 1);
	if (!cleaned_value)
		return (NULL);
	vars.cleaned_len = 0;
	vars.i = 0;
	while (vars.i < vars.len && spaces(var_value[vars.i]) == 1)
	{
		vars.i++;
	}
	spaces_check(var_value, current, &vars, cleaned_value);
	if (vars.cleaned_len > 0 && cleaned_value[vars.cleaned_len - 1] == ' ')
	{
		vars.cleaned_len--;
	}
	cleaned_value[vars.cleaned_len] = '\0';
	return (cleaned_value);
}

void	find_value_var(t_expansion_vars_b *vars, t_input *current)
{
	vars->var_name = ft_strndup(current->str + vars->var_start, vars->var_len);
	if (!vars->var_name)
		return ;
	vars->var_value = ft_getenv(vars->var_name, vars->env);
	free(vars->var_name);
	if (vars->var_value)
	{
		if (vars->in_double_quotes == 1)
		{
			vars->result_len += ft_strlen(vars->var_value);
		}
		else
		{
			vars->cleaned_value = clean_variable_value(vars->var_value,
					vars->current_copy);
			if (vars->cleaned_value)
			{
				vars->result_len += ft_strlen(vars->cleaned_value);
				free(vars->cleaned_value);
			}
		}
	}
	vars->i += vars->var_len;
}

void	expansion(t_expansion_vars_b *vars, t_input *current)
{
	vars->var_start = vars->i + 1;
	vars->var_len = 0;
	while (vars->var_start + vars->var_len < vars->len
		&& ((ft_isalnum(vars->current_copy->str[vars->var_start + vars->var_len])
		|| vars->current_copy->str[vars->var_start + vars->var_len] == '?')
		|| vars->current_copy->str[vars->var_start + vars->var_len] == '_'))
		vars->var_len++;
	if (vars->var_len > 0)
		find_value_var(vars, current);
	else
		vars->result_len++;
}

void	double_quotes(t_expansion_vars_b *vars)
{
	if (vars->in_single_quotes == -1)
		vars->in_double_quotes *= -1;
	else
		vars->result_len++;
}

void	single_quotes(t_expansion_vars_b *vars)
{
	if (vars->in_double_quotes == -1)
		vars->in_single_quotes *= -1;
	else
		vars->result_len++;
}

void	check_char(t_expansion_vars_b *vars, t_input *current)
{
	if (vars->current_copy->str[vars->i] == '\'')
		single_quotes(vars);
	else if (vars->current_copy->str[vars->i] == '"')
		double_quotes(vars);
	else if (vars->current_copy->str[vars->i] == '$' && vars->in_single_quotes == -1
		&& vars->current_copy->type != HEREDOC)
		expansion(vars, current);
	else
		vars->result_len++;
}

t_expansion_vars_b	*init_expansion(t_input *current)
{
	t_expansion_vars_b	*vars;

	vars = malloc(sizeof(t_expansion_vars_b));
	if (!vars)
		return (NULL);
	vars->current_copy = current;
	vars->len = ft_strlen(current->str);
	vars->result_len = 0;
	vars->in_single_quotes = -1;
	vars->in_double_quotes = -1;
	vars->i = 0;
	return (vars);
}

size_t	calculate_expanded_length(t_input *current, t_list *env)
{
	t_expansion_vars_b	*vars;
	size_t				result_len;

	vars = init_expansion(current);
	if (!vars)
		return (0);
	vars->env = env;
	while (vars->i < vars->len)
	{
		check_char(vars, current);
		vars->i++;
	}
	result_len = vars->result_len;
	free(vars);
	return (result_len);
}

void	append_value(expand_vars *vars, char *result)
{
	vars->value_len = ft_strlen(vars->expanded_value);
	vars->j = 0;
	while (vars->j < vars->value_len)
	{
		result[vars->result_index++] = vars->expanded_value[vars->j++];
	}
}
void	expand_variable(expand_vars *vars, t_input *current, char *result)
{
	vars->var_name = ft_strndup(current->str + vars->var_start, vars->var_len);
	if (!vars->var_name)
		return ;
	vars->var_value =ft_getenv(vars->var_name, vars->env);
	if (current->type != CMD && vars->var_value == NULL)
		current->fd = -42;
	free(vars->var_name);
	if (vars->var_value)
	{
		if (vars->in_double_quotes == 1)
		{
			vars->expanded_value = ft_strdup(vars->var_value);
		}
		else
		{
			vars->expanded_value = clean_variable_value(vars->var_value,
					current);
		}
		if (vars->expanded_value)
		{
			append_value(vars, result);
			free(vars->expanded_value);
		}
	}
	vars->i += vars->var_len;
}

void	variable_expansion(expand_vars *vars, t_input *current, char *result)
{
	vars->var_start = vars->i + 1;
	vars->var_len = 0;
	while (vars->var_start + vars->var_len < vars->len
		&& (((ft_isalnum(vars->current_copy->str[vars->var_start + vars->var_len])
			|| vars->current_copy->str[vars->var_start + vars->var_len] == '?')
			|| current->str[vars->var_start + vars->var_len] == '_')))
		vars->var_len++;
	if (vars->var_len > 0)
		expand_variable(vars, current, result);
	else
	{
		result[vars->result_index++] = current->str[vars->i];
	}
}

void	check_character(expand_vars *vars, t_input *current, char *result)
{
	if (current->str[vars->i] == '\'')
		vars->in_single_quotes *= -1;
	else if (current->str[vars->i] == '"')
		vars->in_double_quotes *= -1;
	else if (current->str[vars->i] == '$' && vars->in_single_quotes != 1
		&& current->type != HEREDOC)
		variable_expansion(vars, current, result);
	else
		result[vars->result_index++] = current->str[vars->i];
}

expand_vars	*init_expand_vars(t_input *current, t_list *env)
{
	expand_vars	*vars;

	vars = malloc(sizeof(expand_vars));
	vars->current_copy = current;
	vars->env = env;
	vars->result_len = calculate_expanded_length(current, env);
	vars->len = ft_strlen(current->str);
	vars->in_single_quotes = -1;
	vars->in_double_quotes = -1;
	vars->i = 0;
	return (vars);
}

char	*expand_variables(t_input *current, t_list *env)
{
	expand_vars	*vars;
	char		*result;

	vars = init_expand_vars(current, env);
	if (!vars)
		return (NULL);
	result = malloc(vars->result_len + 1);
	if (!result)
	{
		free(vars);
		return (NULL);
	}
	vars->result_index = 0;
	while (vars->i < vars->len)
	{
		check_character(vars, current, result);
		vars->i++;
	}
	result[vars->result_index] = '\0';
	free(vars);
	return (result);
}

int	is_inside_quotes(const char *str, int index)
{
	int	i;
	int	in_double_quotes;
	int	in_single_quotes;

	in_double_quotes = -1;
	in_single_quotes = -1;
	for (i = 0; i < index; i++)
	{
		if (str[i] == '\"')
		{
			in_double_quotes = -1 * in_double_quotes;
		}
		else if (str[i] == '\'')
		{
			in_single_quotes = -1 * in_single_quotes;
		}
	}
	if (in_double_quotes == 1)
		return (1);
	if (in_single_quotes == 1)
		return (2);
	return (0);
}

char	*quotes_to_special(char *input)
{
	int		length;
	char	*result;
	int		j;
	int		ri;
	int		i;

	ri = 0;
	i = 0;
	length = ft_strlen(input);
	result = (char *)malloc(length + 1);
	while (i < length)
	{
		if ((input[i] == '\"' && i + 1 < length && input[i + 1] == '\"')
			|| (input[i] == '\'' && i + 1 < length && input[i + 1] == '\''))
		{
			if (is_inside_quotes(input, i) != 0)
				result[ri++] = input[i++];
			j = i;
			while ((input[j] == '\"' && j + 1 < length && input[j + 1] == '\"')
				|| (input[j] == '\'' && j + 1 < length && input[j + 1] == '\''))
				j += 2;
			if ((i > 0 && spaces(input[i - 1] == 0)) || ((j < length
					&& spaces(input[j]) == 0)))
				i = j;
			else
			{
				if ((i > 0 && spaces(input[i - 1]) == 1) && (j < length
						&& spaces(input[j]) == 1))
					result[ri++] = '\x1E';
				else if ((i == 0 && spaces(input[j]) == 1) || (i == 0
						&& j == length) || (j == length && spaces(input[i
							- 1]) == 1))
					result[ri++] = '\x1E';
				i = j;
			}
		}
		else
		{
			result[ri++] = input[i];
			i++;
		}
	}
	result[ri] = '\0';
	free(input);
	return (result);
}

void	save_history(int fd, const char *entry)
{
	write(fd, entry, ft_strlen(entry));
	write(fd, "\n", 1);
}
void	heredoc_quotes(t_input *current)
{
	char	*tmp;

	tmp = current->str;
	while (*tmp)
	{
		if (*tmp == '"' || *tmp == '\'')
			current->fd = -420;
		tmp++;
	}
}

void	take_history(t_main_vars *vars)
{
	vars->history_fd = open(".tmp/.history.txt", O_RDONLY);
	if (vars->history_fd != -1)
	{
		vars->line = get_next_line(vars->history_fd);
		while (vars->line)
		{
			vars->line[ft_strlen(vars->line) - 1] = '\0';
			add_history(vars->line);
			free(vars->line);
			vars->line = get_next_line(vars->history_fd);
		}
		get_next_line(-vars->history_fd);
		close(vars->history_fd);
	}
}

void	ask_more_for_quotes(t_main_vars *vars)
{
	vars->temp_input = readline("> ");
	vars->new_input = malloc(ft_strlen(vars->input)
			+ ft_strlen(vars->temp_input) + 2);
	ft_strlcpy(vars->new_input, vars->input, INT_MAX);
	ft_strlcat(vars->new_input, "\n", INT_MAX);
	ft_strlcat(vars->new_input, vars->temp_input, INT_MAX);
	free(vars->input);
	vars->input = vars->new_input;
	free(vars->temp_input);
}

void	ask_more_for_pipes(t_main_vars *vars, int *exit_status)
{
	vars->temp_input = readline("> ");
	if (check_syntax_errors(vars->temp_input) != 0)
	{
		*exit_status = 2;
		free(vars->temp_input);
		return ;
	}
	vars->new_input = malloc(ft_strlen(vars->input)
			+ ft_strlen(vars->temp_input) + 1);
	ft_strlcpy(vars->new_input, vars->input, INT_MAX);
	ft_strlcat(vars->new_input, vars->temp_input, INT_MAX);
	free(vars->input);
	vars->input = vars->new_input;
	free(vars->temp_input);
	while (is_quote_balanced(vars->input) == -1)
		ask_more_for_quotes(vars);
}

void	update_history(t_main_vars *vars)
{
	if (*vars->input)
	{
		vars->history_fd = open(".tmp/.history.txt", O_WRONLY | O_APPEND | O_EXCL,
				0644);
		add_history(vars->input);
		if (vars->history_fd != -1)
		{
			save_history(vars->history_fd, vars->input);
			close(vars->history_fd);
		}
	}
}

void	expand_tokens(t_main_vars *vars, t_list *lst_env)
{
	vars->current = vars->tokens;
	while (vars->current)
	{
		if (vars->current->type == HEREDOC)
			heredoc_quotes(vars->current);
		if (vars->current->type != PIPE)
		{
			vars->expanded_str = expand_variables(vars->current, lst_env);
			if (vars->expanded_str)
			{
				if (vars->current->fd != -42)
				{
					free(vars->current->str);
					vars->current->str = vars->expanded_str;
					vars->expanded_str = NULL;
				}
				else
					free(vars->expanded_str);
			}
		}
		vars->current = vars->current->next;
	}
}

void	free_tokens_and_input(t_main_vars *vars)
{
	while (vars->tokens)
	{
		vars->free_tmp = vars->tokens;
		vars->tokens = vars->tokens->next;
		if (vars->free_tmp->str)
			free(vars->free_tmp->str);
		free(vars->free_tmp);
	}
	free(vars->input);
}

int	main(int argc, char **argv, char **env)
{
	t_main_vars	vars;
	t_list		*lst_env;
	int			exit_status;
	char		*tmp_str;
	char		*tmp2_str;

	exit_status = 0;
	(void)argc;
	(void)argv;
	take_history(&vars);
	lst_env = ft_matrix_to_lst(env);
	if (ft_getenv("OLDPWD", lst_env) == NULL)
		ft_export("OLDPWD", &lst_env, 0);
	int tmp = dup(STDIN_FILENO); // per avere tutto perfettamente pulito
	dup2(tmp, STDIN_FILENO); // per avere tutto perfettamente pulito
	close(tmp); // per avere tutto perfettamente pulito
	tmp = dup(STDOUT_FILENO); // per avere tutto perfettamente pulito
	dup2(tmp, STDOUT_FILENO); // per avere tutto perfettamente pulito
	close(tmp); // per avere tutto perfettamente pulito
	while (1)
	{
		vars.input = readline("minicecco> ");
		if (!vars.input)
			break ;
		if (vars.input[0] == '\0')
		{
			free(vars.input);
			continue ;
		}
		if (check_syntax_errors(vars.input) != 0)
		{
			update_history(&vars);
			exit_status = 2;
			free(vars.input);
			continue;
		}
		while (is_quote_balanced(vars.input) == -1)
		{
			ask_more_for_quotes(&vars);
			if (check_syntax_errors(vars.input) != 0)
			{
				update_history(&vars);
				exit_status = 2;
				free(vars.input);
				continue;
			}
		}
		while (is_pipe_balanced(vars.input) == -1)
			ask_more_for_pipes(&vars, &exit_status);
		update_history(&vars);
		if (check_syntax_errors(vars.input) != 0)
		{
			update_history(&vars);
			exit_status = 2;
			free(vars.input);
			continue;
		}
		tmp2_str = ft_itoa(exit_status);
		tmp_str = ft_strjoin("?=", tmp2_str);
		free(tmp2_str);
		ft_export(tmp_str, &lst_env, 1);
		free(tmp_str);
		vars.input = quotes_to_special(vars.input);
		vars.tokens = tokenize(vars.input);
		expand_tokens(&vars, lst_env);
		//print_tokens(vars.tokens);
		//free_tokens_and_input(&vars);
		free(vars.input);
		executer(&vars.tokens, &lst_env, &exit_status);
	}
	close(STDIN_FILENO); // per avere tutto perfettamente pulito
	close(STDOUT_FILENO); // per avere tutto perfettamente pulito
	ft_lstclear(&lst_env, free);
	return (0);
}
