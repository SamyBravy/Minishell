/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdell-er <sdell-er@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 11:43:46 by sdell-er          #+#    #+#             */
/*   Updated: 2024/08/30 11:43:46 by sdell-er         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	only_one_cmd(t_input *input)
{
	while (input)
	{
		if (input->type == PIPE)
			return (0);
		input = input->next;
	}
	return (1);
}

int	get_flags(t_type type)
{
	if (type == TRUNC)
		return (O_CREAT | O_WRONLY | O_TRUNC);
	if (type == APPEND)
		return (O_CREAT | O_WRONLY | O_APPEND);
	if (type == INPUT || type == HEREDOC)
		return (O_RDONLY);
	return (0);
}

char	*get_block_cmd(t_input *input)
{
	while (input && input->type != PIPE)
	{
		if (input->type == CMD)
			return (input->str);
		input = input->next;
	}
	return (NULL);
}

t_builtin	which_builtin(t_input *input)
{
	char	*cmd;

	cmd = get_block_cmd(input);
	if (!cmd)
		return (NONE);
	if (!ft_strncmp(cmd, "echo", 4) && (cmd[4] == '\x1D' || cmd[4] == '\0'))
		return (ECHO);
	if (!ft_strncmp(cmd, "cd", 2) && (cmd[2] == '\x1D' || cmd[2] == '\0'))
		return (CD);
	if (!ft_strncmp(cmd, "pwd", 3) && (cmd[3] == '\x1D' || cmd[3] == '\0'))
		return (PWD);
	if (!ft_strncmp(cmd, "export", 6) && (cmd[6] == '\x1D' || cmd[6] == '\0'))
		return (EXPORT);
	if (!ft_strncmp(cmd, "unset", 5) && (cmd[5] == '\x1D' || cmd[5] == '\0'))
		return (UNSET);
	if (!ft_strncmp(cmd, "env", 3) && (cmd[3] == '\x1D' || cmd[3] == '\0'))
		return (ENV);
	if (!ft_strncmp(cmd, "exit", 4) && (cmd[4] == '\x1D' || cmd[4] == '\0'))
		return (EXIT);
	return (INTERNAL);
}
