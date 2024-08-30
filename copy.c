/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   copy.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgrisost <fgrisost@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 13:47:54 by fgrisost          #+#    #+#             */
/*   Updated: 2024/08/30 13:55:40 by fgrisost         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strndup(char *s, size_t n)
{
	char	*src;
	char	*copy;
	size_t	i;

	src = s;
	i = 0;
	if (n < ft_strlen(s))
		copy = malloc(sizeof(char) * (n + 1));
	else
		copy = malloc(sizeof(char) * (ft_strlen(s) + 1));
	while (src[i] && i < n)
	{
		copy[i] = src[i];
		i++;
	}
	copy[i] = '\0';
	return (copy);
}

void	dollar_check(t_vars_samu *vars, char *str)
{
	while (vars->var_start + vars->var_len < vars->len && (str[vars->var_start
				+ vars->var_len] != '>' && str[vars->var_start
				+ vars->var_len] != '<' && str[vars->var_start
				+ vars->var_len] != '|' && str[vars->var_start
				+ vars->var_len] != '"' && str[vars->var_start
				+ vars->var_len] != '\'' && str[vars->var_start
				+ vars->var_len] != '\n' && str[vars->var_start
				+ vars->var_len] != ' ' && str[vars->var_start
				+ vars->var_len] != '$'))
	{
		vars->var_len++;
		if (str[vars->var_start] == '?')
			break ;
	}
	if (vars->var_len > 0)
	{
		vars->var_name = ft_strndup(str + vars->var_start, vars->var_len);
		vars->var_value = ft_getenv(vars->var_name, vars->env);
		free(vars->var_name);
		if (vars->var_value)
			vars->result_len += strlen(vars->var_value);
		vars->i += vars->var_len;
	}
	else
		vars->result_len++;
}

size_t	calculate_length_samu(char *str, t_list *env)
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
			dollar_check(&vars, str);
		}
		else
			vars.result_len++;
		vars.i++;
	}
	return (vars.result_len);
}

void	copy_value(t_vars_samu *vars, char *result)
{
	size_t	value_len;
	size_t	i;

	value_len = ft_strlen(vars->var_value);
	i = 0;
	while (i < value_len)
		result[vars->result_len++] = vars->var_value[i++];
}

void	expand(t_vars_samu *vars, char *str, char *result)
{
	vars->var_name = ft_strndup(str + vars->var_start, vars->var_len);
	vars->var_value = ft_getenv(vars->var_name, vars->env);
	free(vars->var_name);
	if (vars->var_value)
		copy_value(vars, result);
}

void	check_dollar_help(t_vars_samu *vars, char *str, char *result)
{
	while (vars->var_start + vars->var_len < vars->len && (str[vars->var_start
				+ vars->var_len] != '>' && str[vars->var_start
				+ vars->var_len] != '<' && str[vars->var_start
				+ vars->var_len] != '|' && str[vars->var_start
				+ vars->var_len] != '"' && str[vars->var_start
				+ vars->var_len] != '\'' && str[vars->var_start
				+ vars->var_len] != '\n' && str[vars->var_start
				+ vars->var_len] != ' ' && str[vars->var_start
				+ vars->var_len] != '$'))
	{
		vars->var_len++;
		if (str[vars->var_start] == '?')
			break ;
	}
	if (vars->var_len > 0)
	{
		expand(vars, str, result);
		vars->i += vars->var_len;
	}
	else
		result[vars->result_len++] = str[vars->i];
}

void	check_dollar(t_vars_samu *vars, char *str, char *result)
{
	if (str[vars->i] == '$')
	{
		vars->var_start = vars->i + 1;
		vars->var_len = 0;
		check_dollar_help(vars, str, result);
	}
	else
		result[vars->result_len++] = str[vars->i];
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

char	*expand_samu(char *str, t_list *env)
{
	t_vars_samu	*vars;
	size_t		expanded_length;
	char		*result;

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
	return (result);
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
	}
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
		while (temp->next)
			temp = temp->next;
		temp->next = new_node;
	}
}

t_type	identify_type(char *token)
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

void	special_char(t_input **head, t_token_variables *vars)
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
		if (vars->cmd_str)
		{
			add_node(head, new_node(CMD, vars->cmd_str));
			free(vars->cmd_str);
			vars->cmd_str = NULL;
		}
		add_node(head, new_node(vars->current_type, NULL));
	}
}

void	quotes(t_token_variables *vars)
{
	vars->c_for_quotes = *vars->copy_str;
	vars->start = vars->copy_str++;
	while (*vars->copy_str && *vars->copy_str != vars->c_for_quotes)
		vars->copy_str++;
	vars->copy_str++;
	vars->token = ft_strndup(vars->start, vars->copy_str - vars->start);
}

void	normal_char(t_token_variables *vars)
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
}

void	save_cmd(t_token_variables *vars)
{
	char	*help;

	if (vars->cmd_str)
	{
		help = vars->copy_str - ft_strlen(vars->token) - 1;
		vars->temp_cmd_str = calloc(ft_strlen(vars->cmd_str)
				+ ft_strlen(vars->token) + 2, 1);
		ft_strlcpy(vars->temp_cmd_str, vars->cmd_str, INT_MAX);
		if (spaces(*help) == 1)
			ft_strlcat(vars->temp_cmd_str, "\x1D", INT_MAX);
		ft_strlcat(vars->temp_cmd_str, vars->token, INT_MAX);
		free(vars->cmd_str);
		vars->cmd_str = vars->temp_cmd_str;
	}
	else
		vars->cmd_str = ft_strndup(vars->token, INT_MAX);
}

void	save_special(t_input **head, t_token_variables *vars)
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

void	save_not_pipe(t_input **head, t_token_variables *vars)
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

t_token_variables	*prepare_var(char *str)
{
	t_token_variables	*vars;

	vars = malloc(sizeof(t_token_variables));
	vars->token = NULL;
	vars->current_type = CMD;
	vars->cmd_str = NULL;
	vars->copy_str = str;
	vars->flag = 0;
	return (vars);
}

t_input	*tokenize(char *str)
{
	t_input				*head;
	t_token_variables	*vars;

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

int	is_quote_balanced(const char *str, char *quotes)
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
	*quotes = '"' * (in_double_quote == 1) + '\'' * in_single_quote == 1;
	if (in_double_quote == -1 && in_single_quote == -1)
		return (1);
	return (-1);
}

int	is_pipe_balanced(char *str)
{
	char	*ptr;

	ptr = str;
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

void	skip_spaces(char *var_value, t_size_t_clean *vars)
{
	while (vars->i < vars->len && spaces(var_value[vars->i]) == 1)
		vars->i++;
}

void	spaces_check(char *var_value, t_input *current, t_size_t_clean *vars,
		char *cleaned_value)
{
	int	j;

	j = 0;
	while (spaces(var_value[vars->i]) == 1)
		vars->i++;
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
			j = vars->i;
			while (var_value[j] && spaces(var_value[j]) == 1)
				j++;
			if (var_value[j] == '\0')
				break ;
		}
	}
}

char	*clean_variable_value(char *var_value, t_input *current)
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
		vars.i++;
	spaces_check(var_value, current, &vars, cleaned_value);
	if (vars.cleaned_len > 0 && cleaned_value[vars.cleaned_len - 1] == ' ')
		vars.cleaned_len--;
	cleaned_value[vars.cleaned_len] = '\0';
	return (cleaned_value);
}

void	find_value_var(t_expansion_vars_b *vars, t_input *current)
{
	vars->var_name = ft_strndup(current->str + vars->var_start, vars->var_len);
	vars->var_value = ft_getenv(vars->var_name, vars->env);
	free(vars->var_name);
	if (vars->var_value)
	{
		if (vars->in_double_quotes == 1)
			vars->result_len += ft_strlen(vars->var_value);
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

void	expansion_help(t_expansion_vars_b *vars)
{
	while (vars->var_start + vars->var_len < vars->len
		&& ((vars->current_copy->str[vars->var_start + vars->var_len] != '>'
				&& vars->current_copy->str[vars->var_start
					+ vars->var_len] != '<'
				&& vars->current_copy->str[vars->var_start
					+ vars->var_len] != '|'
				&& vars->current_copy->str[vars->var_start
					+ vars->var_len] != '"'
				&& vars->current_copy->str[vars->var_start
					+ vars->var_len] != '\''
				&& vars->current_copy->str[vars->var_start
					+ vars->var_len] != '\x1d'
				&& vars->current_copy->str[vars->var_start
					+ vars->var_len] != '$'
				&& vars->current_copy->str[vars->var_start
					+ vars->var_len] != ' ')))
	{
		vars->var_len++;
		if (vars->current_copy->str[vars->var_start] == '?')
			break ;
	}
}

void	expansion(t_expansion_vars_b *vars, t_input *current)
{
	vars->var_start = vars->i + 1;
	vars->var_len = 0;
	expansion_help(vars);
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
	if (vars->current_copy->str[vars->i] == '\'' && vars->in_double_quotes
		== -1)
		single_quotes(vars);
	else if (vars->current_copy->str[vars->i] == '"'
		&& vars->in_single_quotes == -1)
		double_quotes(vars);
	else if (vars->current_copy->str[vars->i] == '$'
		&& vars->in_single_quotes == -1 && vars->current_copy->type != HEREDOC)
		expansion(vars, current);
	else
		vars->result_len++;
}

t_expansion_vars_b	*init_expansion(t_input *current)
{
	t_expansion_vars_b	*vars;

	vars = malloc(sizeof(t_expansion_vars_b));
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

void	append_value(t_expand_vars *vars, char *result)
{
	vars->value_len = ft_strlen(vars->expanded_value);
	vars->j = 0;
	while (vars->j < vars->value_len)
		result[vars->result_index++] = vars->expanded_value[vars->j++];
}

void	expand_variable(t_expand_vars *vars, t_input *current, char *result)
{
	vars->var_name = ft_strndup(current->str + vars->var_start, vars->var_len);
	vars->var_value = ft_getenv(vars->var_name, vars->env);
	if (current->type != CMD && vars->var_value == NULL)
		current->fd = -42;
	free(vars->var_name);
	if (vars->var_value)
	{
		if (vars->in_double_quotes == 1)
			vars->expanded_value = ft_strdup(vars->var_value);
		else
			vars->expanded_value = clean_variable_value(vars->var_value,
					current);
		if (vars->expanded_value)
		{
			append_value(vars, result);
			free(vars->expanded_value);
		}
	}
	vars->i += vars->var_len;
}

void	variable_expansion_help(t_expand_vars *vars)
{
	while (vars->var_start + vars->var_len < vars->len
		&& ((vars->current_copy->str[vars->var_start + vars->var_len] != '>'
				&& vars->current_copy->str[vars->var_start
					+ vars->var_len] != '<'
				&& vars->current_copy->str[vars->var_start
					+ vars->var_len] != '|'
				&& vars->current_copy->str[vars->var_start
					+ vars->var_len] != '"'
				&& vars->current_copy->str[vars->var_start
					+ vars->var_len] != '\''
				&& vars->current_copy->str[vars->var_start
					+ vars->var_len] != '\x1d'
				&& vars->current_copy->str[vars->var_start
					+ vars->var_len] != '$'
				&& vars->current_copy->str[vars->var_start
					+ vars->var_len] != ' ')))
	{
		vars->var_len++;
		if (vars->current_copy->str[vars->var_start] == '?')
			break ;
	}
}

void	variable_expansion(t_expand_vars *vars, t_input *current, char *result)
{
	vars->var_start = vars->i + 1;
	vars->var_len = 0;
	variable_expansion_help(vars);
	if (vars->var_len > 0)
		expand_variable(vars, current, result);
	else
		result[vars->result_index++] = current->str[vars->i];
}

void	check_character(t_expand_vars *vars, t_input *current, char *result)
{
	if (current->str[vars->i] == '\'' && vars->in_double_quotes == -1)
		vars->in_single_quotes *= -1;
	else if (current->str[vars->i] == '"' && vars->in_single_quotes == -1)
		vars->in_double_quotes *= -1;
	else if (current->str[vars->i] == '$' && vars->in_single_quotes != 1
		&& current->type != HEREDOC)
		variable_expansion(vars, current, result);
	else
		result[vars->result_index++] = current->str[vars->i];
}

t_expand_vars	*init_t_expand_vars(t_input *current, t_list *env)
{
	t_expand_vars	*vars;

	vars = malloc(sizeof(t_expand_vars));
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
	t_expand_vars	*vars;
	char			*result;

	vars = init_t_expand_vars(current, env);
	result = malloc(vars->result_len + 1);
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
	i = 0;
	while (i < index)
	{
		if (str[i] == '\"' && in_single_quotes == -1)
			in_double_quotes = -1 * in_double_quotes;
		else if (str[i] == '\'' && in_double_quotes == -1)
			in_single_quotes = -1 * in_single_quotes;
		i++;
	}
	if (in_double_quotes == 1)
		return (1);
	if (in_single_quotes == 1)
		return (2);
	return (0);
}

void	change_quotes(t_quotes_special_vars *vars, char *input)
{
	vars->j = vars->i;
	while ((input[vars->j] == '\"' && vars->j + 1 < vars->length
			&& input[vars->j + 1] == '\"') || (input[vars->j] == '\'' && vars->j
			+ 1 < vars->length && input[vars->j + 1] == '\''))
		vars->j += 2;
	if ((vars->i > 0 && spaces(input[vars->i - 1] == 0))
		|| ((vars->j < vars->length && spaces(input[vars->j]) == 0)))
		vars->i = vars->j;
	else
	{
		if ((vars->i > 0 && spaces(input[vars->i - 1]) == 1)
			&& (vars->j < vars->length && spaces(input[vars->j]) == 1))
			vars->result[vars->ri++] = '\x1E';
		else if ((vars->i == 0 && spaces(input[vars->j]) == 1) || (vars->i == 0
				&& vars->j == vars->length) || (vars->j == vars->length
				&& spaces(input[vars->i - 1]) == 1))
			vars->result[vars->ri++] = '\x1E';
		vars->i = vars->j;
	}
}

char	*quotes_to_special(char *input)
{
	t_quotes_special_vars	vars;

	vars.ri = 0;
	vars.i = 0;
	vars.length = ft_strlen(input);
	vars.result = (char *)malloc(vars.length + 1);
	while (vars.i < vars.length)
	{
		if ((input[vars.i] == '\"' && vars.i + 1 < vars.length && input[vars.i
					+ 1] == '\"') || (input[vars.i] == '\'' && vars.i
				+ 1 < vars.length && input[vars.i + 1] == '\''))
		{
			if (is_inside_quotes(input, vars.i) != 0)
			{
				vars.result[vars.ri++] = input[vars.i++];
				continue ;
			}
			change_quotes(&vars, input);
		}
		else
			vars.result[vars.ri++] = input[vars.i++];
	}
	vars.result[vars.ri] = '\0';
	free(input);
	return (vars.result);
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

void	update_history(t_main_vars *vars)
{
	if (*vars->input)
	{
		vars->history_fd = open(".tmp/.history.txt",
				O_WRONLY | O_APPEND | O_EXCL, 0644);
		add_history(vars->input);
		if (vars->history_fd != -1)
		{
			save_history(vars->history_fd, vars->input);
			close(vars->history_fd);
		}
	}
}

void	take_history(t_main_vars *vars)
{
	vars->history_fd = open(".tmp/.history.txt", O_RDONLY);
	if (vars->history_fd == -1)
	{
		vars->history_fd = open(".tmp/.history.txt", O_CREAT | O_WRONLY, 0644);
		close(vars->history_fd);
		vars->history_fd = open(".tmp/.history.txt", O_RDONLY);
	}
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

bool	more_quotes_help(t_main_vars *vars, char quotes)
{
	update_history(vars);
	free(vars->input);
	vars->input = NULL;
	ft_putstr_fd("minicecco: unexpected EOF while looking for matching `",
		STDERR_FILENO);
	ft_putchar_fd(quotes, STDERR_FILENO);
	ft_putstr_fd("'\nminicecco: syntax error: unexpected end of file\n",
		STDERR_FILENO);
	return (false);
}

bool	ask_more_for_quotes(t_main_vars *vars, char quotes)
{
	vars->temp_input = readline("> ");
	if (g_signal == SIGINT)
	{
		update_history(vars);
		free(vars->input);
		vars->input = NULL;
		return (false);
	}
	if (!vars->temp_input)
		return (more_quotes_help(vars, quotes));
	vars->new_input = malloc(ft_strlen(vars->input)
			+ ft_strlen(vars->temp_input) + 2);
	ft_strlcpy(vars->new_input, vars->input, INT_MAX);
	ft_strlcat(vars->new_input, "\n", INT_MAX);
	ft_strlcat(vars->new_input, vars->temp_input, INT_MAX);
	free(vars->input);
	vars->input = vars->new_input;
	free(vars->temp_input);
	return (true);
}

int	more_pipes_help(t_main_vars *vars)
{
	update_history(vars);
	free(vars->input);
	vars->input = NULL;
	ft_putstr_fd("minicecco: syntax error: unexpected end of file\n",
		STDERR_FILENO);
	return (0);
}

int	ask_more_for_pipes(t_main_vars *vars)
{
	char	quotes;

	vars->temp_input = readline("> ");
	if (g_signal == SIGINT)
	{
		update_history(vars);
		free(vars->input);
		vars->input = NULL;
		return (0);
	}
	if (!vars->temp_input)
		return (more_pipes_help(vars));
	vars->new_input = malloc(ft_strlen(vars->input)
			+ ft_strlen(vars->temp_input) + 1);
	ft_strlcpy(vars->new_input, vars->input, INT_MAX);
	ft_strlcat(vars->new_input, vars->temp_input, INT_MAX);
	free(vars->input);
	vars->input = vars->new_input;
	free(vars->temp_input);
	while (is_quote_balanced(vars->input, &quotes) == -1)
	{
		if (!ask_more_for_quotes(vars, quotes))
			return (2);
	}
	return (1);
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

int	only_spaces(const char *str)
{
	if (str == NULL || *str == '\0')
		return (0);
	while (*str != '\0')
	{
		if (*str != '\x1d')
			return (0);
		str++;
	}
	return (1);
}

char	*remove_spaces(char *str)
{
	char	*start;
	char	*str_no_spaces;

	start = str;
	while (*start == '\x1d')
		start++;
	str_no_spaces = calloc(ft_strlen(start) + 1, sizeof(char));
	ft_strlcpy(str_no_spaces, start, INT_MAX);
	free(str);
	return (str_no_spaces);
}

void	remove_empty_nodes_help(t_input **current, t_input **prev)
{
	*prev = *current;
	*current = (*current)->next;
}

void	remove_empty_nodes(t_input **head)
{
	t_input	*current;
	t_input	*prev;
	t_input	*temp;

	current = *head;
	prev = NULL;
	while (current != NULL)
	{
		if (current->type == CMD)
			current->str = remove_spaces(current->str);
		if (current->str != NULL && (current->str[0] == '\0'
				|| only_spaces(current->str) == 1))
		{
			temp = current;
			if (prev == NULL)
				*head = current->next;
			else
				prev->next = current->next;
			current = current->next;
			free(temp->str);
			free(temp);
		}
		else
			remove_empty_nodes_help(&current, &prev);
	}
}

void	tokenize_and_ex(t_main_vars *vars, int *exit_status, t_list **lst_env)
{
	vars->tmp2_str = ft_itoa(*exit_status);
	vars->tmp_str = ft_strjoin("?=", vars->tmp2_str);
	free(vars->tmp2_str);
	ft_export(vars->tmp_str, lst_env, 1);
	free(vars->tmp_str);
	vars->input = quotes_to_special(vars->input);
	vars->tokens = tokenize(vars->input);
	expand_tokens(vars, *lst_env);
	remove_empty_nodes(&vars->tokens);
	free(vars->input);
	executer(&vars->tokens, lst_env, exit_status);
}

int	read_and_signal(t_main_vars *vars, int *exit_status)
{
	g_signal = 0;
	signal(SIGINT, handel_sig_def);
	signal(SIGQUIT, handel_sig_def);
	vars->input = readline("minicecco> ");
	if (g_signal == SIGINT)
	{
		g_signal = 0;
		*exit_status = 130;
		free(vars->input);
		return (1);
	}
	if (!vars->input)
	{
		ft_putstr_fd("exit\n", STDERR_FILENO);
		return (2);
	}
	if (vars->input[0] == '\0')
	{
		free(vars->input);
		return (1);
	}
	return (0);
}

int	syntax_and_quotes(t_main_vars *vars, int *exit_status, char *quotes)
{
	if (check_syntax_errors(vars->input) != 0)
	{
		update_history(vars);
		*exit_status = 2;
		free(vars->input);
		return (1);
	}
	while (is_quote_balanced(vars->input, quotes) == -1)
	{
		if (!ask_more_for_quotes(vars, *quotes))
		{
			if (g_signal == SIGINT)
			{
				g_signal = 0;
				*exit_status = 130;
			}
			else
				*exit_status = 2;
			break ;
		}
	}
	if (!vars->input)
		return (1);
	return (0);
}

int	balanced_pipes_help(t_main_vars *vars, int *exit_status)
{
	if (!ask_more_for_pipes(vars))
	{
		if (g_signal == SIGINT)
		{
			g_signal = 0;
			*exit_status = 130;
		}
		else
		{
			ft_putstr_fd("exit\n", STDERR_FILENO);
			*exit_status = 2;
		}
		return (2);
	}
	else if (!vars->input)
	{
		if (g_signal == SIGINT)
			*exit_status = 130;
		else
			*exit_status = -42;
		g_signal = 0;
		return (2);
	}
	return (0);
}

int	update_history_and_syntax(t_main_vars *vars, int *exit_status,
		t_list **lst_env)
{
	if (!vars->input && *exit_status == 2)
		return (2);
	else if (!vars->input && *exit_status == 130)
		return (1);
	else if (!vars->input)
	{
		*exit_status = 2;
		return (2);
	}
	update_history(vars);
	if (check_syntax_errors(vars->input) != 0)
	{
		*exit_status = 2;
		free(vars->input);
		return (1);
	}
	tokenize_and_ex(vars, exit_status, lst_env);
	return (0);
}

void	main_loop(t_main_vars *vars, int *exit_status, t_list **lst_env,
		char *quotes)
{
	while (1)
	{
		vars->ret = read_and_signal(vars, exit_status);
		if (vars->ret == 1)
			continue ;
		else if (vars->ret == 2)
			break ;
		if (syntax_and_quotes(vars, exit_status, quotes))
			continue ;
		while (is_pipe_balanced(vars->input) == -1)
		{
			if (balanced_pipes_help(vars, exit_status) == 2)
				break ;
		}
		vars->ret = update_history_and_syntax(vars, exit_status, lst_env);
		if (vars->ret == 1)
			continue ;
		else if (vars->ret == 2)
			break ;
	}
}

int	main(int argc, char **argv, char **env)
{
	t_main_vars	vars;
	t_list		*lst_env;
	int			exit_status;
	char		quotes;

	g_signal = 0;
	exit_status = 0;
	(void)argc;
	(void)argv;
	take_history(&vars);
	lst_env = ft_matrix_to_lst(env);
	if (ft_getenv("OLDPWD", lst_env) == NULL)
		ft_export("OLDPWD", &lst_env, 0);
	vars.tmp = dup(STDIN_FILENO);
	dup2(vars.tmp, STDIN_FILENO);
	close(vars.tmp);
	vars.tmp = dup(STDOUT_FILENO);
	dup2(vars.tmp, STDOUT_FILENO);
	close(vars.tmp);
	main_loop(&vars, &exit_status, &lst_env, &quotes);
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	ft_lstclear(&lst_env, free);
	return (exit_status);
}
