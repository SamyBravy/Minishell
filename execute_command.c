/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 19:40:52 by marvin            #+#    #+#             */
/*   Updated: 2024/08/13 19:40:52 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_free_mat(char **mat)
{
	int	i;

	if (!mat)
		return ;
	i = 0;
	while (mat[i])
		free(mat[i++]);
	free(mat);
}

static char	*realloc_with_path(char *path)
{
	char	*cmd_name;
	char	**possible_paths;
	char	*tmp;
	int		i;

	if (!getenv("PATH"))
		return (free(path), NULL);
	possible_paths = ft_split(getenv("PATH"), ':');
	cmd_name = path;
	i = -1;
	while (possible_paths[++i])
	{
		tmp = ft_strjoin(possible_paths[i], "/");
		path = ft_strjoin(tmp, cmd_name);
		free(tmp);
		if (access(path, F_OK) == 0)
		{
			free(cmd_name);
			ft_free_mat(possible_paths);
			return (path);
		}
		free(path);
	}
	ft_free_mat(possible_paths);
	return (free(cmd_name), NULL);
}

static int	parse_internal_cmd(char *str_cmd, t_cmd *cmd)
{
	int		len;

	len = ft_strlen(str_cmd);
	if (strchr(str_cmd, ' '))
		len = ft_strchr(str_cmd, ' ') - str_cmd;
	cmd->path = ft_substr(str_cmd, 0, len);
	if (!strchr(cmd->path, '/'))
	{
		cmd->path = realloc_with_path(cmd->path);
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

int	exec_builtin(t_input **input, t_cmd *cmd)
{
	t_builtin	builtin;
	char		**argv;
	int			exit_status;

	if (cmd->fd_in == -1)
		return (1);
	argv = ft_split(get_block_cmd(*input), ' ');
	builtin = which_builtin(*input);
	exit_status = 0;
	// if (builtin == ECHO)
	// 	exit_status = builtin_echo(argv, argv[1] && !ft_strcmp(argv[1], "-n"));	// se c'è echo -n, la stringa da stampare inizia dopo echo -n
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
	// 	exit_status = builtin_exit(argv, input);	// farà il free di argv e di input
	(void) builtin;
	return (ft_free_mat(argv), exit_status);
}

void	exec_cmd(t_input **input, t_cmd *cmd)
{
	if (which_builtin(*input) != INTERNAL)
		free_and_exit(input, exec_builtin(input, cmd));
	if (cmd->fd_in == -1)
		free_and_exit(input, 1);
	if (parse_internal_cmd(get_block_cmd(*input), cmd) == -1)
		free_and_exit(input, 127);
	if (execve(cmd->path, cmd->argv, cmd->env) == -1)	// forse prima va fatto il free di input
	{
		ft_putstr_fd("minicecco: ", 2);
		perror(cmd->path);
		free(cmd->path);	// non so se va davvero fatto
		ft_free_mat(cmd->argv); // non so se va davvero fatto
		while (*input)	// non so se va davvero fatto
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
