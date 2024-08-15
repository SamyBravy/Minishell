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

void	close_pipe(int *pipefd)
{
	if (pipefd[0] != -1)
		close(pipefd[0]);
	if (pipefd[1] != -1)
		close(pipefd[1]);
	pipefd[0] = -1;
	pipefd[1] = -1;
}

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

void	clean_and_exit(t_input **input, int exit_status, int *pipefd)
{
	while (*input)
		clean_block(input, pipefd == NULL);
	if (pipefd)
	{
		close_pipe(pipefd);
		close(STDIN_FILENO);
		close(STDOUT_FILENO);
	}
	exit(exit_status);
}
