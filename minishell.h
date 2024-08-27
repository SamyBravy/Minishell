/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdell-er <sdell-er@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 15:20:23 by sdell-er          #+#    #+#             */
/*   Updated: 2024/08/10 19:28:59 by sdell-er         ###   ########.fr       */
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

extern int	g_signal;

/* executer */
void		*executer(t_input **input, t_list **env, int *exit_status);

void		create_heredocs(t_input *input);

void		exec_cmd(t_input **input, t_cmd *cmd, t_list **env);
int			exec_builtin(t_input **input, t_cmd *cmd,
				t_int_list **std_inout_pipes, t_list **env);

void		exit_error(t_input **input, t_list **env,
				t_int_list **std_inout_pipes);
void		clean_int_list(t_int_list **lst);
void		clean_block(t_input **input, int unlink_heredoc);
void		clean_and_exit(t_input **input, t_list **env, int exit_status,
				int forked);

t_builtin	which_builtin(t_input *input);
char		*get_block_cmd(t_input *input);
int			get_flags(t_type type);
int			only_one_cmd(t_input *input);

void		handle_sig_heredoc(int sig);
void		handle_sig_execve(int sig);

/* builtins */
int			exit_builtin(char **argv, t_input **input, t_list **env,
				t_int_list **std_inout_pipes);

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
int			ft_export(char *str, t_list **env);
int			is_valid_identifier(char *str);
void		lst_remove_key(t_list **env, char *key);

/* parsing */
int			check_syntax_errors(char *token);
t_type		identify_type(char *token);

#endif
