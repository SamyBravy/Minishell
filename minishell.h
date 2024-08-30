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

extern int	g_signal;

/* executer */
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

void		handel_sig_def(int sig);
void		handle_sig_execve(int sig);

/* builtins */
int			exit_builtin(char **argv, t_input **input, t_list **env,
				t_int_list **stdio_pipes_fds);

int			echo_builtin(char **argv);

int			pwd_builtin(void);

int			env_builtin(t_list *env);

int			unset_builtin(char **argv, t_list **env);

int			export_builtin(char **argv, t_list **env);
void		export_append(char *str, char *key, t_list **env);

int			cd_builtin(char **argv, t_list **env);

/* env_utils */
char		*ft_getenv(char *key, t_list *env);
char		*get_key(char *str);
int			ft_export(char *str, t_list **env, int check);
int			is_valid_identifier(char *str, int check);
void		lst_remove_key(t_list **env, char *key);

/* parsing */
int			check_syntax_errors(char *token);
t_type		identify_type(char *token);

char		*expand_samu(char *str, t_list *env);
t_input	*new_node(t_type type, char *str);
void	find_lst_cmd(t_input *temp, t_input **last_cmd);
void	concatenate_str(t_input *new_node, t_input *temp, t_input *last_cmd, char *temp_str);
void	add_node(t_input **head, t_input *new_node);
t_type	identify_type(char *token);
char	*clean_variable_value(char *var_value, t_input *current);
void	spaces_check(char *var_value, t_input *current, t_size_t_clean *vars, char *cleaned_value);
void	skip_spaces(char *var_value, t_size_t_clean *vars);
int	is_pipe_balanced(char *str);
int	is_quote_balanced(const char *str, char *quotes);
void	normal_char(t_token_variables *vars);
void	quotes(t_token_variables *vars);
void	special_char(t_input **head, t_token_variables *vars);
int	spaces(char c);
char	*expand_variables(t_input *current, t_list *env);
t_expand_vars	*init_t_expand_vars(t_input *current, t_list *env);
void	check_character(t_expand_vars *vars, t_input *current, char *result);
void	variable_expansion(t_expand_vars *vars, t_input *current, char *result);
void	variable_expansion_help(t_expand_vars *vars);
void	double_quotes(t_expansion_vars_b *vars);
void	expansion(t_expansion_vars_b *vars, t_input *current);
void	expansion_help(t_expansion_vars_b *vars);
void	find_value_var(t_expansion_vars_b *vars, t_input *current);
void	check_char(t_expansion_vars_b *vars, t_input *current);
t_expansion_vars_b	*init_expansion(t_input *current);
size_t	calculate_expanded_length(t_input *current, t_list *env);
void	append_value(t_expand_vars *vars, char *result);
void	expand_variable(t_expand_vars *vars, t_input *current, char *result);
void	check_dollar_help(t_vars_samu *vars, char *str, char *result);
void	expand(t_vars_samu *vars, char *str, char *result);
void	copy_value(t_vars_samu *vars, char *result);
size_t	calculate_length_samu(char *str, t_list *env);
void	dollar_check(t_vars_samu *vars, char *str);
char	*ft_strndup(char *s, size_t n);
void	check_dollar(t_vars_samu *vars, char *str, char *result);
t_vars_samu	*init_expand_samu(const char *str);
char	*expand_samu(char *str, t_list *env);
void	heredoc_quotes(t_input *current);
char	*quotes_to_special(char *input);
void	change_quotes(t_quotes_special_vars *vars, char *input);
int	is_inside_quotes(const char *str, int index);
int	update_history_and_syntax(t_main_vars *vars, int *exit_status, t_list **lst_env);
void	take_history(t_main_vars *vars);
void	update_history(t_main_vars *vars);
void	save_history(int fd, const char *entry);
void	main_loop(t_main_vars *vars, int *exit_status, t_list **lst_env, char *quotes);
int	balanced_pipes_help(t_main_vars *vars, int *exit_status);
int	syntax_and_quotes(t_main_vars *vars, int *exit_status, char *quotes);
int	read_and_signal(t_main_vars *vars, int *exit_status);
void	tokenize_and_ex(t_main_vars *vars, int *exit_status, t_list **lst_env);
bool	more_quotes_help(t_main_vars *vars, char quotes);
bool	ask_more_for_quotes(t_main_vars *vars, char quotes);
int	more_pipes_help(t_main_vars *vars);
int	ask_more_for_pipes(t_main_vars *vars);
void	remove_empty_nodes_help(t_input **current, t_input **prev);
void	remove_empty_nodes(t_input **head);
void	expand_tokens(t_main_vars *vars, t_list *lst_env);
void	free_tokens_and_input(t_main_vars *vars);
int	just_spaces(const char *str);
char	*remove_spaces(char *str);
t_input	*tokenize(char *str);
t_token_variables	*prepare_var(char *str);
void	save_not_pipe(t_input **head, t_token_variables *vars);
void	save_special(t_input **head, t_token_variables *vars);
void	save_cmd(t_token_variables *vars);
void	single_quotes(t_expansion_vars_b *vars);





#endif
