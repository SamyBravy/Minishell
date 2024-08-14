/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_signals.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 19:41:03 by marvin            #+#    #+#             */
/*   Updated: 2024/08/13 19:41:03 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handle_sig_heredoc(int sig)
{
	if (sig == SIGINT)
	{
		g_signal = SIGINT;
		ioctl(STDIN_FILENO, TIOCSTI, "\n");
	}
	else if (sig == SIGQUIT)
		ft_putstr_fd("\b\b  \b\b", STDERR_FILENO);
}

void	handle_sig_execve(int sig)
{
	if (sig == SIGINT)
	{
		ft_putchar_fd('\n', STDOUT_FILENO);
		g_signal = SIGINT;
	}
	else if (sig == SIGQUIT)
	{
		ft_putstr_fd("Quit (core dumped)\n", STDERR_FILENO);
		g_signal = SIGQUIT;
	}
}
