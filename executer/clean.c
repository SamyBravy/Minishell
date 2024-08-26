/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_and_clean.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 19:40:47 by marvin            #+#    #+#             */
/*   Updated: 2024/08/13 19:40:47 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	close_pipefd(int *pipefd)
{
	close(pipefd[0]);
	close(pipefd[1]);
}

void	exit_error(t_input **input, t_list **env, int original_stdin)
{
	close(original_stdin);
	clean_and_exit(input, env, 1, 0);
}

static void	close_block_fd(t_input *input)
{
	while (input && input->type != PIPE)
	{
		if (input->type == INPUT || input->type == HEREDOC
			|| input->type == TRUNC || input->type == APPEND)
		{
			if (input->fd < 0)
				return ;
			close(input->fd);
		}
		input = input->next;
	}
}

void	clean_block(t_input **input, int unlink_heredoc)
{
	t_input	*tmp;

	close_block_fd(*input);
	while (*input && (*input)->type != PIPE)
	{
		if ((*input)->type == HEREDOC && unlink_heredoc)
			unlink((*input)->str);
		free((*input)->str);
		tmp = *input;
		*input = (*input)->next;
		free(tmp);
	}
	if (*input && (*input)->type == PIPE)
	{
		tmp = *input;
		*input = (*input)->next;
		free(tmp);
	}
}

void	clean_and_exit(t_input **input, t_list **env, int exit_status,
	int forked)
{
	while (input && *input)
		clean_block(input, !forked);
	close(STDIN_FILENO);
	if (forked)
		close(STDOUT_FILENO);
	ft_lstclear(env, free);
	exit(exit_status);
}
