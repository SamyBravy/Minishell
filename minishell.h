/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgrisost <fgrisost@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 14:10:43 by fgrisost          #+#    #+#             */
/*   Updated: 2024/08/30 14:10:43 by fgrisost         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdbool.h>
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <signal.h>
# include <limits.h>
# include <fcntl.h>
# include <errno.h>
# include <sys/wait.h>
# include <sys/ioctl.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "my_lib/Libft/libft.h"
# include "my_lib/ft_printf/ft_printf.h"
# include "my_lib/get_next_line/get_next_line_bonus.h"

# define PROMPT "\033[0;95mminicecco\033[0m:"
# define PROMPT_PIPE "\033[0;31m|\033[0m> "
# define PROMPT_SINGLE_QUOTE "\033[0;33m'\033[0m> "
# define PROMPT_DOUBLE_QUOTE "\033[0;38;5;214m\"\033[0m> "

extern int	g_signal;

/* Executer */
typedef enum e_builtin
{
	INTERNAL,
	NONE,
	ECHO,
	CD,
	PWD,
	EXPORT,
	UNSET,
	ENV,
	EXIT
}	t_builtin;

typedef enum e_type
{
	CMD,
	TRUNC,
	APPEND,
	INPUT,
	HEREDOC,
	PIPE,
}	t_type;

typedef struct s_input
{
	t_type			type;
	char			*str;
	int				fd;
	struct s_input	*next;
}	t_input;

typedef struct s_cmd
{
	char	*path;
	char	**argv;
	char	**env;
	int		fd_in;
	int		fd_out;
}	t_cmd;

/* Parsing */
typedef struct s_token_variables
{
	char	*token;
	t_type	current_type;
	char	*cmd_str;
	char	*temp_cmd_str;
	char	temp[3];
	int		i;
	char	c_for_quotes;
	char	*start;
	char	*copy_str;
	int		flag;
}	t_token_variables;

typedef struct s_expand_vars
{
	t_input	*current_copy;
	size_t	result_len;
	size_t	result_index;
	size_t	len;
	int		in_single_quotes;
	int		in_double_quotes;
	size_t	var_start;
	size_t	var_len;
	char	*var_name;
	char	*var_value;
	char	*expanded_value;
	size_t	value_len;
	size_t	i;
	size_t	j;
	t_list	*env;
}	t_expand_vars;

typedef struct s_expansion_vars_b
{
	t_input	*current_copy;
	size_t	len;
	size_t	result_len;
	int		in_single_quotes;
	int		in_double_quotes;
	size_t	i;
	size_t	var_start;
	size_t	var_len;
	char	*var_name;
	char	*var_value;
	char	*cleaned_value;
	t_list	*env;
}	t_expansion_vars_b;

typedef struct s_clean
{
	size_t	len;
	size_t	cleaned_len;
	size_t	i;
}	t_size_t_clean;

typedef struct s_main_vars
{
	char	*input;
	char	*temp_input;
	t_input	*tokens;
	char	*expanded_str;
	char	*new_input;
	t_input	*current;
	int		history_fd;
	char	*line;
	t_input	*free_tmp;
	char	*tmp_str;
	char	*tmp2_str;
	int		tmp;
	int		ret;
}	t_main_vars;

typedef struct s_quotes_special_vars
{
	int		length;
	char	*result;
	int		j;
	int		ri;
	int		i;
}	t_quotes_special_vars;

typedef struct s_vars_samu
{
	size_t			i;
	size_t			len;
	size_t			var_start;
	size_t			var_len;
	size_t			result_len;
	char			*var_name;
	const char		*var_value;
	t_list			*env;
}	t_vars_samu;

/* Executer */
void		*executer(t_input **input, t_list **env, int *exit_status);

void		create_heredocs(t_input *input, t_list *env);

void		exec_cmd(t_input **input, t_cmd *cmd, t_list **env);
int			exec_builtin(t_input **input, t_cmd *cmd,
				t_int_list **stdio_pipes_fds, t_list **env);

void		exit_error(t_input **input, t_list **env,
				t_int_list **stdio_pipes_fds);
void		clean_int_list(t_int_list **lst);
void		clean_block(t_input **input, int unlink_heredoc);
void		clean_and_exit(t_input **input, t_list **env, int exit_status,
				int forked);

t_builtin	which_builtin(t_input *input);
char		*get_block_cmd(t_input *input);
int			get_flags(t_type type);
int			only_one_cmd(t_input *input);

/* Builtins */
int			exit_builtin(char **argv, t_input **input, t_list **env,
				t_int_list **stdio_pipes_fds);

int			echo_builtin(char **argv);

int			pwd_builtin(void);

int			env_builtin(t_list *env);

int			unset_builtin(char **argv, t_list **env);

int			export_builtin(char **argv, t_list **env);
void		export_append(char *str, char *key, t_list **env);

int			cd_builtin(char **argv, t_list **env);

/* General */
char		*ft_getenv(char *key, t_list *env);
char		*get_key(char *str);
int			ft_export(char *str, t_list **env, int check);
int			is_valid_identifier(char *str, int check);
void		lst_remove_key(t_list **env, char *key);

void		handel_sig_def(int sig);
void		handle_sig_execve(int sig);

/* Parsing */
int			check_syntax_errors(char *token);
t_type		identify_type(char *token);
char		*expand_samu(char *str, t_list *env);
t_input		*new_node(t_type type, char *str);
void		add_node(t_input **head, t_input *new_node);
t_type		identify_type(char *token);
char		*clean_variable_value(char *var_value, t_input *current);
int			is_pipe_balanced(char *str);
int			is_quote_balanced(const char *str, char *quotes);
void		normal_char(t_token_variables *vars);
void		quotes(t_token_variables *vars);
void		special_char(t_input **head, t_token_variables *vars);
int			spaces(char c);
char		*expand_variables(t_input *current, t_list *env);
void		variable_expansion(t_expand_vars *vars, t_input *current,
				char *result);
void		double_quotes(t_expansion_vars_b *vars);
void		expansion(t_expansion_vars_b *vars, t_input *current);
void		expansion_help(t_expansion_vars_b *vars);
void		check_char(t_expansion_vars_b *vars, t_input *current);
size_t		calculate_expanded_length(t_input *current, t_list *env);
void		expand_variable(t_expand_vars *vars, t_input *current,
				char *result);
void		check_dollar_help(t_vars_samu *vars, char *str, char *result);
void		expand(t_vars_samu *vars, char *str, char *result);
size_t		calculate_length_samu(char *str, t_list *env);
char		*ft_strndup(char *s, size_t n);
void		check_dollar(t_vars_samu *vars, char *str, char *result);
char		*expand_samu(char *str, t_list *env);
void		heredoc_quotes(t_input *current);
char		*quotes_to_special(char *input);
int			update_history_and_syntax(t_main_vars *vars, int *exit_status,
				t_list **lst_env);
void		take_history(t_main_vars *vars);
void		update_history(t_main_vars *vars);
void		main_loop(t_main_vars *vars, int *exit_status,
				t_list **lst_env);
void		tokenize_and_ex(t_main_vars *vars, int *exit_status,
				t_list **lst_env);
bool		ask_more_for_quotes(t_main_vars *vars, char quotes);
int			more_pipes_help(t_main_vars *vars);
int			ask_more_for_pipes(t_main_vars *vars);
void		remove_empty_nodes(t_input **head);
void		dollar_check_help_samu(t_vars_samu *vars);
void		expand_tokens(t_main_vars *vars, t_list *lst_env);
int			just_spaces(const char *str);
char		*remove_spaces(char *str);
t_input		*tokenize(char *str);
void		single_quotes(t_expansion_vars_b *vars);

#endif
