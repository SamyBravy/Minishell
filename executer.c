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

#include "minishell.h"

char	*unique_name(void)
{
	static int	n = 0;
	char		*str;
	char		*tmp;

	tmp = ft_itoa(n);
	str = ft_strjoin(".tmp/.", tmp);
	free(tmp);
	n++;
	return (str);
}

void	read_heredoc(int fd, char *eof)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!line || !ft_strcmp(line, eof))
		{
			if (line)
				free(line);
			else
			{
				ft_printf("minicecco: warning: here-document ");
				ft_printf("delimited by end-of-file (wanted `%s')\n", eof);
			}
			break ;
		}
		ft_putstr_fd(line, fd);
		ft_putchar_fd('\n', fd);
		free(line);
	}
}

void	create_heredocs(t_input *input)
{
	char	*eof;

	while (input)
	{
		if (input->type == HEREDOC)
		{
			eof = input->str;
			input->fd = -1;
			while (input->fd == -1)
			{
				input->str = unique_name();
				input->fd = open(input->str, O_CREAT | O_EXCL | O_WRONLY, 0644);
				if (input->fd == -1)
					free(input->str);
			}
			read_heredoc(input->fd, eof);
			free(eof);
			close(input->fd);
		}
		input = input->next;
	}
}

void	open_block_files(t_input *input, t_cmd *cmd)
{
	while (input && input->type != PIPE)
	{
		if (input->type == INPUT || input->type == HEREDOC)
		{
			input->fd = open(input->str, O_RDONLY);
			cmd->fd_in = input->fd;
			if (input->fd == -1)
			{
				ft_printf("minicecco: %s: No such file or directory\n",
					input->str);
				return ;
			}
		}
		else if (input->type == TRUNC)
		{
			input->fd = open(input->str, O_CREAT | O_TRUNC | O_WRONLY, 0644);
			cmd->fd_out = input->fd;
		}
		else if (input->type == APPEND)
		{
			input->fd = open(input->str, O_CREAT | O_APPEND | O_WRONLY, 0644);
			cmd->fd_out = input->fd;
		}
		input = input->next;
	}
}

void	remove_head(t_input **input)
{
	t_input	*tmp;

	tmp = *input;
	*input = (*input)->next;
	free(tmp);
}

void	close_block_fd(t_input *input)
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
	if (!ft_strncmp(cmd, "echo", 4) && (cmd[4] == ' ' || cmd[4] == '\0'))
		return (ECHO);
	if (!ft_strncmp(cmd, "cd", 2) && (cmd[2] == ' ' || cmd[2] == '\0'))
		return (CD);
	if (!ft_strncmp(cmd, "pwd", 3) && (cmd[3] == ' ' || cmd[3] == '\0'))
		return (PWD);
	if (!ft_strncmp(cmd, "export", 6) && (cmd[6] == ' ' || cmd[6] == '\0'))
		return (EXPORT);
	if (!ft_strncmp(cmd, "unset", 5) && (cmd[5] == ' ' || cmd[5] == '\0'))
		return (UNSET);
	if (!ft_strncmp(cmd, "env", 3) && (cmd[3] == ' ' || cmd[3] == '\0'))
		return (ENV);
	if (!ft_strncmp(cmd, "exit", 4) && (cmd[4] == ' ' || cmd[4] == '\0'))
		return (EXIT);
	return (INTERNAL);
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

char	*realloc_with_path(char **path)
{
	char	*cmd_name;
	char	**possible_paths;
	char	*tmp;
	int		i;

	if (!getenv("PATH"))
		return (free(path), NULL);
	possible_paths = ft_split(getenv("PATH"), ':');
	cmd_name = *path;
	i = -1;
	while (possible_paths[++i])
	{
		tmp = ft_strjoin(possible_paths[i], "/");
		*path = ft_strjoin(tmp, cmd_name);
		free(tmp);
		if (access(*path, F_OK) == 0)
		{
			free(cmd_name);
			ft_free_mat(possible_paths);
			return *path;
		}
		free(*path);
	}
	ft_free_mat(possible_paths);
	return (free(cmd_name), NULL);
}

int	parse_internal_cmd(char *str_cmd, t_cmd *cmd)
{
	int		len;

	len = ft_strlen(str_cmd);
	if (strchr(str_cmd, ' '))
		len = ft_strchr(str_cmd, ' ') - str_cmd;
	cmd->path = ft_substr(str_cmd, 0, len);
	if (!strchr(cmd->path, '/'))
	{
		cmd->path = realloc_with_path(&cmd->path);
		if (!cmd->path)
		{
			ft_putstr_fd("minicecco: ", 2);
			write(2, str_cmd, len);
			ft_putstr_fd(": command not found\n", 2);
			cmd->argv = NULL;
			return (-1);
		}
		cmd->argv = ft_split(str_cmd, ' ');
		free(cmd->argv[0]);
		cmd->argv[0] = ft_strdup(cmd->path);
	}
	else
		cmd->argv = ft_split(str_cmd, ' ');
	return (0);
}

int	exec_builtin(t_input **input, t_cmd cmd, char **env)
{
	t_builtin	builtin;
	char		**argv;
	int			exit_status;

	if (cmd.fd_in == -1)
		return (1);
	argv = ft_split(get_block_cmd(*input), ' ');
	builtin = which_builtin(*input);
	exit_status = 0;
	// if (builtin == ECHO)
	// 	exit_status = builtin_echo(argv, ft_strnstr(get_block_cmd(*input),	// controllare errori sulla lunghezza nel caso in cui la stringa è più corta di echo -n
	// 				"echo -n ", ft_strlen(get_block_cmd(*input))));	// se c'è echo -n, la stringa da stampare inizia dopo echo -n
	// else if (builtin == CD)
	// 	exit_status = builtin_cd(argv);
	// else if (builtin == PWD)
	// 	exit_status = builtin_pwd(argv);
	// else if (builtin == EXPORT)
	// 	exit_status = builtin_export(argv, env);
	// else if (builtin == UNSET)
	// 	exit_status = builtin_unset(argv, env);
	// else if (builtin == ENV)
	// 	exit_status = builtin_env(argv, env);
	// else if (builtin == EXIT)
	// 	exit_status = builtin_exit(&argv, input);	// farà il free di argv e di input
	(void) cmd;
	(void) env;
	(void) builtin;
	return (ft_free_mat(argv), exit_status);
}

void	free_and_exit(t_input **input, int exit_status)
{
	while (*input)
		clean_block(input, 0);
	exit(exit_status);
}

void	exec_cmd(t_input **input, t_cmd cmd, char **env)
{
	if (which_builtin(*input) != INTERNAL)
		free_and_exit(input, exec_builtin(input, cmd, env));
	if (cmd.fd_in == -1)
		free_and_exit(input, 1);
	if (parse_internal_cmd(get_block_cmd(*input), &cmd) == -1)
		free_and_exit(input, 127);
	if (execve(cmd.path, cmd.argv, env) == -1)
	{
		ft_putstr_fd("minicecco: ", 2);
		ft_putstr_fd(cmd.path, 2);
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(strerror(errno), 2);
		ft_putchar_fd('\n', 2);
		free(cmd.path);	// non so se va davvero fatto
		ft_free_mat(cmd.argv); // non so se va davvero fatto
		while (*input)
			clean_block(input, 0);
		if (errno == ENOENT)	// Comando non trovato
			exit(127);
		if (errno == EACCES || errno == ENOEXEC || errno == EISDIR	// Permesso negato, Formato file non valido, È una directory
			|| errno == ELOOP || errno == ENOTDIR || errno == EPERM	// Troppi link simbolici, Non è una directory, Operazione non permessa
			|| errno == EROFS || errno == ETXTBSY)	// File system di sola lettura, File di testo occupato
			exit(126);
		exit(1);
	}
}

void	executer(t_input *input, char **env, int *exit_status)
{
	int		i;
	int		pid;
	int		pipefd[2];
	int		original_stdin;
	t_cmd	cmd;

	create_heredocs(input);
	original_stdin = dup(STDIN_FILENO);
	i = 0;
	while (input)
	{
		cmd.fd_in = STDIN_FILENO;
		cmd.fd_out = STDOUT_FILENO;
		open_block_files(input, &cmd);
		if (!i && only_one_cmd(input) && which_builtin(input) != INTERNAL)
			*exit_status = exec_builtin(&input, cmd, env);
		else
		{
			pipe(pipefd);	// controllare errori
			if (!only_one_cmd(input) && cmd.fd_out == STDOUT_FILENO)
				cmd.fd_out = pipefd[1];
			pid = fork();	// controllare errori
			if (pid == 0)
			{
				dup2(cmd.fd_in, STDIN_FILENO);
				dup2(cmd.fd_out, STDOUT_FILENO);
				close(pipefd[0]);
				close(pipefd[1]);
				exec_cmd(&input, cmd, env);
			}
			dup2(pipefd[0], STDIN_FILENO);
			close(pipefd[0]);
			close(pipefd[1]);
			i++;
		}
		clean_block(&input, 1);
	}
	if (i--)
		waitpid(pid, exit_status, 0);
	if (WIFEXITED(*exit_status))
		*exit_status = WEXITSTATUS(*exit_status);
	else
		*exit_status = 0;
	while (i-- > 0)
		waitpid(-1, NULL, 0);
	dup2(original_stdin, STDIN_FILENO);
	close(original_stdin);
}
