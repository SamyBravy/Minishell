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

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <signal.h>
# include <fcntl.h>
# include <errno.h>
# include <sys/wait.h>
# include <sys/ioctl.h>
# include <readline/readline.h>
# include "my_lib/Libft/libft.h"
# include "my_lib/ft_printf/ft_printf.h"
# include "my_lib/get_next_line/get_next_line_bonus.h"

# define TRUE 1

typedef enum e_builtin
{
	INTERNAL,	// 0, non è un builtin
	NONE,	// non ci sono comandi in quel blocco
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
	TRUNC,	// >
	APPEND,	// >>
	INPUT,	// <
	HEREDOC,	// <<
	PIPE,	// |
}	t_type;

typedef struct s_input // arriva all'executer
{
	t_type			type;
	char			*str;
	int				fd;	// questo lo crea l'executer
	struct s_input	*next;
}	t_input;

typedef struct s_cmd // creata dall'executer
{
	char	*path;
	char	**argv;
	char	**env;
	int		fd_in;
	int		fd_out;
}	t_cmd;

int	g_signal;

void		executer(t_input **input, char **env, int *exit_status);
void		clean_block(t_input **input, int unlink_heredoc);

void		create_heredocs(t_input *input);

void		exec_cmd(t_input **input, t_cmd *cmd);
int			exec_builtin(t_input **input, t_cmd *cmd);

void		open_block_files(t_input *input, t_cmd *cmd);
void		clean_block(t_input **input, int unlink_heredoc);
void		free_and_exit(t_input **input, int exit_status);

t_builtin	which_builtin(t_input *input);
char		*get_block_cmd(t_input *input);
int			only_one_cmd(t_input *input);

void		handle_sig_heredoc(int sig);
void		handle_sig_execve(int sig);

#endif
