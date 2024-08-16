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

#include "../minishell.h"

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
			ft_putstr_fd("minicecco: ", STDERR_FILENO);
			write(2, str_cmd, len);
			ft_putstr_fd(": command not found\n", STDERR_FILENO);
			cmd->argv = NULL;
			return (-1);
		}
	}
	cmd->argv = ft_split(str_cmd, '\x1D');
	return (0);
}

int	exec_builtin(t_input **input, t_cmd *cmd,
	t_int_list **pipes_stdin_fds, t_list **env)
{
	t_builtin	builtin;
	char		**argv;
	int			exit_status;

	if (cmd->fd_in == -1)
		return (1);
	argv = ft_split(get_block_cmd(*input), '\x1D');
	builtin = which_builtin(*input);
	exit_status = 0;
	if (builtin == ECHO)
		exit_status = echo_builtin(argv);
	/*else if (builtin == CD)
		exit_status = cd_builtin(argv, env);*/
	else if (builtin == PWD)
		exit_status = pwd_builtin();
	/*else if (builtin == EXPORT)
		exit_status = export_builtin(argv, env);*/
	else if (builtin == UNSET)
		exit_status = unset_builtin(argv, env);
	else if (builtin == ENV)
		exit_status = env_builtin(env);
	if (builtin == EXIT)
		exit_status = exit_builtin(argv, input, env, pipes_stdin_fds);
	return (ft_free_mat(argv), exit_status);
}

static void	insert_null_char(char **argv)
{
	int	i;

	i = 1;
	while (argv[i])
	{
		if (argv[i][0] == '\x1E')
			argv[i][0] = '\0';
		i++;
	}
}

void	exec_cmd(t_input **input, t_cmd *cmd, t_list **env)
{
	int	exit_status;

	if (which_builtin(*input) != INTERNAL)
		clean_and_exit(input, env, exec_builtin(input, cmd, NULL, env), 1);
	if (cmd->fd_in == -1)
		clean_and_exit(input, env, 1, 1);
	if (parse_internal_cmd(get_block_cmd(*input), cmd) == -1)
		clean_and_exit(input, env, 127, 1);
	insert_null_char(cmd->argv);
	cmd->env = ft_lst_to_matrix(*env);
	if (execve(cmd->path, cmd->argv, cmd->env) == -1)
	{
		ft_putstr_fd("minicecco: ", STDERR_FILENO);
		perror(cmd->path);
		free(cmd->path);
		ft_free_mat(cmd->argv);
		ft_free_mat(cmd->env);
		exit_status = 1;
		if (errno == 2)
			exit_status = 127;
		if (errno == 13 || errno == 8 || errno == 21 || errno == 40 || errno
			== 20 || errno == 1 || errno == 30 || errno == 26 || errno == 16)
			exit_status = 126;
		clean_and_exit(input, env, exit_status, 1);
	}
}
