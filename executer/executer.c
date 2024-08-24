/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdell-er <sdell-er@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 09:10:44 by sdell-er          #+#    #+#             */
/*   Updated: 2024/08/11 18:36:03 by sdell-er         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	open_block_files(t_input *input, t_cmd *cmd)
{
	while (input && input->type != PIPE)
	{
		if (input->type == INPUT || input->type == HEREDOC)
		{
			if (input->fd != -42)
				input->fd = open(input->str, O_RDONLY);
			cmd->fd_in = input->fd;
			if (input->fd == -1 || input->fd == -42)
			{
				ft_putstr_fd("minicecco: ", STDERR_FILENO);
				ft_putstr_fd(input->str, STDERR_FILENO);
				if (input->fd == -1)
					perror("\x1E");
				else
					ft_putstr_fd(": ambiguous redirect\n", STDERR_FILENO);
				return ;
			}
		}
		else if (input->type == TRUNC || input->type == APPEND)
		{
			input->fd = open(input->str, get_flags(input->type), 0644);
			cmd->fd_out = input->fd;
		}
		input = input->next;
	}
}

static void	init_signals_and_heredocs(t_input **input, int *exit_status)
{
	g_signal = 0;
	signal(SIGINT, handle_sig_heredoc);
	signal(SIGQUIT, handle_sig_heredoc);
	create_heredocs(*input);
	if (g_signal == SIGINT)
	{
		*exit_status = 130;
		while (*input)
			clean_block(input, 1);
		return ;
	}
	signal(SIGINT, handle_sig_execve);
	signal(SIGQUIT, handle_sig_execve);
}

static void	set_exit_status(int i, int pid, int *exit_status)
{
	if (i--)
	{
		waitpid(pid, exit_status, 0);
		if (WIFEXITED(*exit_status))
			*exit_status = WEXITSTATUS(*exit_status);
		else
			*exit_status = 0;
	}
	if (g_signal == SIGINT)
		*exit_status = 130;
	else if (g_signal == SIGQUIT)
		*exit_status = 131;
	while (i-- > 0)
		waitpid(-1, NULL, 0);
}

static int	create_pipe_and_fork(t_input **input, t_cmd *cmd, t_list **env,
	t_int_list **pipes_stdin_fds)
{
	int	pid;
	int	pipefd[2];

	if (pipe(pipefd) == -1)
		exit_error(input, env, pipes_stdin_fds);
	ft_lstadd_back_int(pipes_stdin_fds, pipefd[0]);
	if (!only_one_cmd(*input) && cmd->fd_out == STDOUT_FILENO)
		cmd->fd_out = pipefd[1];
	pid = fork();
	if (pid == -1)
	{
		close(pipefd[1]);
		exit_error(input, env, pipes_stdin_fds);
	}
	if (pid == 0)
	{
		dup2(cmd->fd_in, STDIN_FILENO);
		dup2(cmd->fd_out, STDOUT_FILENO);
		close(pipefd[1]);
		clean_int_list(pipes_stdin_fds);
		exec_cmd(input, cmd, env);
	}
	dup2(pipefd[0], STDIN_FILENO);
	close(pipefd[1]);
	return (pid);
}

void	executer(t_input **input, t_list **env, int *exit_status)
{
	int			i;
	int			pid;
	t_int_list	*pipes_stdin_fds;
	t_cmd		cmd;

	init_signals_and_heredocs(input, exit_status);
	pipes_stdin_fds = ft_new_intlst(dup(STDIN_FILENO));
	//close(STDIN_FILENO);	// per avere tutto perfettamente pulito (non funziona se la metto)
	i = 0;
	while (*input)
	{
		cmd.fd_in = STDIN_FILENO;
		cmd.fd_out = STDOUT_FILENO;
		open_block_files(*input, &cmd);
		if (!i && only_one_cmd(*input) && which_builtin(*input) != INTERNAL)
			*exit_status = exec_builtin(input, &cmd, &pipes_stdin_fds, env);
		else if (i++ || TRUE)
			pid = create_pipe_and_fork(input, &cmd, env, &pipes_stdin_fds);
		clean_block(input, 1);
	}
	set_exit_status(i, pid, exit_status);
	dup2(pipes_stdin_fds->content, STDIN_FILENO);
	clean_int_list(&pipes_stdin_fds);
}
