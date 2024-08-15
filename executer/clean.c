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

void	ft_free_mat(char **mat)
{
	int	i;

	if (!mat)
		return ;
	i = 0;
	while (mat[i])
		free(mat[i++]);
	free(mat);
}

static void	remove_head(t_input **input)
{
	t_input	*tmp;

	tmp = *input;
	*input = (*input)->next;
	free(tmp);
}

static void	close_block_fd(t_input *input)
{
	while (input && input->type != PIPE)
	{
		if (input->type == INPUT || input->type == HEREDOC
			|| input->type == TRUNC || input->type == APPEND)
		{
			if (input->fd == -1)
				return ;
			close(input->fd);
		}
		input = input->next;
	}
}

void	clean_block(t_input **input, int unlink_heredoc)
{
	close_block_fd(*input);
	while (*input && (*input)->type != PIPE)
	{
		if ((*input)->type == HEREDOC && unlink_heredoc)
			unlink((*input)->str);
		free((*input)->str);
		remove_head(input);
	}
	if (*input && (*input)->type == PIPE)
		remove_head(input);
}

void	clean_and_exit(t_input **input, int exit_status, int *pipefd)
{
	if (pipefd)
	{
		close(pipefd[0]);
		close(pipefd[1]);
	}
	while (*input)
		clean_block(input, 0);
	exit(exit_status);
}
