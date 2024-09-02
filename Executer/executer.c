/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdell-er <sdell-er@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 11:43:42 by sdell-er          #+#    #+#             */
/*   Updated: 2024/08/30 11:43:42 by sdell-er         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	g_signal = 0;

static void	open_block_files(t_input *input, t_cmd *cmd)
{
	cmd->fd_in = STDIN_FILENO;
	cmd->fd_out = STDOUT_FILENO;
	while (input && input->type != PIPE)
	{
		if (input->type != CMD)
		{
			if (input->fd != -42)
				input->fd = open(input->str, get_flags(input->type), 0644);
			if (input->type == TRUNC || input->type == APPEND)
				cmd->fd_out = input->fd;
			else
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
		input = input->next;
	}
}

static void	init_signals_and_heredocs(t_input **input, int *exit_status,
	t_list *env)
{
	g_signal = 0;
	signal(SIGINT, handel_sig_def);
	signal(SIGQUIT, handel_sig_def);
	create_heredocs(*input, env);
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
	t_int_list **stdio_pipes_fds, int first)
{
	static int	last_pipe_0;
	int			pipeline[2];
	int			pid;

	if (first)
		last_pipe_0 = -1;
	pipe(pipeline);
	if (last_pipe_0 != -1 && cmd->fd_in == STDIN_FILENO)
		cmd->fd_in = last_pipe_0;
	if (cmd->fd_out == STDOUT_FILENO && !only_one_cmd(*input))
		cmd->fd_out = pipeline[1];
	pid = fork();
	if (pid == 0)
	{
		close(pipeline[0]);
		if (cmd->fd_in != STDIN_FILENO)
			dup2(cmd->fd_in, STDIN_FILENO);
		if (last_pipe_0 != -1)
			close(last_pipe_0);
		if (cmd->fd_out != STDOUT_FILENO)
			dup2(cmd->fd_out, STDOUT_FILENO);
		close(pipeline[1]);
		clean_int_list(stdio_pipes_fds);
		exec_cmd(input, cmd, env);
	}
	close(pipeline[1]);
	if (last_pipe_0 != -1)
		close(last_pipe_0);
	last_pipe_0 = pipeline[0];
	return (pid);
}

void	*executer(t_input **input, t_list **env, int *exit_status)
{
	int			i;
	int			pid;
	t_int_list	*stdio_pipes_fds;
	t_cmd		cmd;

	init_signals_and_heredocs(input, exit_status, *env);
	stdio_pipes_fds = ft_new_intlst(dup(STDIN_FILENO));
	ft_lstadd_back_int(&stdio_pipes_fds, dup(STDOUT_FILENO));
	i = 0;
	while (*input)
	{
		open_block_files(*input, &cmd);
		if (!i && only_one_cmd(*input) && which_builtin(*input) != INTERNAL)
		{
			dup2(cmd.fd_out, STDOUT_FILENO);
			*exit_status = exec_builtin(input, &cmd, &stdio_pipes_fds, env);
		}
		else if (i++ || true)
			pid = create_pipe_and_fork(input, &cmd, env, &stdio_pipes_fds, i == 1);
		clean_block(input, 1);
	}
	return (set_exit_status(i, pid, exit_status), close(STDIN_FILENO),
		close(STDOUT_FILENO), dup2(stdio_pipes_fds->content, STDIN_FILENO),
		dup2(stdio_pipes_fds->next->content, STDOUT_FILENO),
		clean_int_list(&stdio_pipes_fds), NULL);
}
