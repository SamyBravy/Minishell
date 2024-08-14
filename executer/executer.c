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
			input->fd = open(input->str, O_RDONLY);
			cmd->fd_in = input->fd;
			if (input->fd == -1)
			{
				ft_putstr_fd("minicecco: ", STDERR_FILENO);
				perror(input->str);
				return ;
			}
		}
		else if (input->type == TRUNC || input->type == APPEND)
		{
			if (input->type == TRUNC)
				input->fd = open(input->str,
						O_CREAT | O_TRUNC | O_WRONLY, 0644);
			else
				input->fd = open(input->str,
						O_CREAT | O_APPEND | O_WRONLY, 0644);
			cmd->fd_out = input->fd;
		}
		input = input->next;
	}
}

void	init_signals_and_heredocs(t_input **input, int *exit_status)
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

void	set_exit_status(int i, int pid, int *exit_status)
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

void	create_pipe_and_fork(t_input **input, t_cmd *cmd,
	int original_stdin, int *pid)
{
	int	pipefd[2];

	if (pipe(pipefd) == -1)
	{
		close(original_stdin);
		free_and_exit(input, 1);
	}
	if (!only_one_cmd(*input) && cmd->fd_out == STDOUT_FILENO)
		cmd->fd_out = pipefd[1];
	*pid = fork();
	if (*pid == -1 || *pid == 0)
		close(original_stdin);
	if (*pid == -1)
		free_and_exit(input, 1);
	if (*pid == 0)
	{
		dup2(cmd->fd_in, STDIN_FILENO);
		dup2(cmd->fd_out, STDOUT_FILENO);
		close(pipefd[0]);
		close(pipefd[1]);
		exec_cmd(input, cmd);
	}
	dup2(pipefd[0], STDIN_FILENO);
	close(pipefd[0]);
	close(pipefd[1]);
}

void	executer(t_input **input, char **env, int *exit_status)
{
	int		i;
	int		pid;
	int		original_stdin;
	t_cmd	cmd;

	init_signals_and_heredocs(input, exit_status);
	original_stdin = dup(STDIN_FILENO);
	cmd.env = env;
	i = 0;
	while (*input)
	{
		cmd.fd_in = STDIN_FILENO;
		cmd.fd_out = STDOUT_FILENO;
		open_block_files(*input, &cmd);
		if (!i && only_one_cmd(*input) && which_builtin(*input) != INTERNAL)
			*exit_status = exec_builtin(input, &cmd, 0);
		else if (i++ || TRUE)
			create_pipe_and_fork(input, &cmd, original_stdin, &pid);
		clean_block(input, 1);
	}
	set_exit_status(i, pid, exit_status);
	dup2(original_stdin, STDIN_FILENO);
	close(original_stdin);
}
