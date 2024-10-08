/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdell-er <sdell-er@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 11:43:58 by sdell-er          #+#    #+#             */
/*   Updated: 2024/08/30 11:43:58 by sdell-er         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	clean_int_list(t_int_list **lst)
{
	t_int_list	*tmp;

	while (*lst)
	{
		close((*lst)->content);
		tmp = *lst;
		*lst = (*lst)->next;
		free(tmp);
	}
}

void	exit_error(t_input **input, t_list **env, t_int_list **stdio_pipes_fds)
{
	clean_int_list(stdio_pipes_fds);
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
	close(STDOUT_FILENO);
	ft_lstclear(env, free);
	exit(exit_status);
}
