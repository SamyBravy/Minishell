/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdell-er <sdell-er@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 15:20:23 by sdell-er          #+#    #+#             */
/*   Updated: 2024/08/07 09:31:09 by sdell-er         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <signal.h>
# include <readline/readline.h>
# include "my_lib/Libft/libft.h"
# include "my_lib/ft_printf/ft_printf.h"
# include "my_lib/get_next_line/get_next_line_bonus.h"

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
	struct s_input	*next;
}	t_input;

typedef struct s_cmd // creata dall'executer
{
	char	*path;	// /bin/grep
	char	**argv;	// grep -v e = {grep, -v, e, NULL}
	int		fd_in;	// dup2(fd_in, STDIN_FILENO)
	int		fd_out;	// dup2(fd_out, STDOUT_FILENO)
}	t_cmd;	// execve(path, argv, env)

#endif
