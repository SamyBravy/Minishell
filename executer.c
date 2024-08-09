/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdell-er <sdell-er@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 09:10:44 by sdell-er          #+#    #+#             */
/*   Updated: 2024/08/09 17:57:29 by sdell-er         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*unique_name(void)
{
	static int	n = 0;
	char		*str;
	char		*tmp;

	tmp = ft_itoa(n);
	str = ft_strjoin('.', tmp);
	free(tmp);
	n++;
	return (str);
}

void	read_heredoc(int fd, char *eof)
{
	char	*line;
	int		i;

	i = 1;
	while (1)
	{
		line = readline("> ");
		if (!line || !ft_strcmp(line, eof))
		{
			if (line)
				free(line);
			else
				ft_printf("minicecco: warning: here-document at line %d \
					delimited by end-of-file (wanted `%s')\n", i, eof);
			break ;
		}
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
		i++;
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
				ft_printf("minishell: %s: No such file or directory\n",
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

void	close_fd(t_input *input)
{
	while (input)
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

void	clean_block(t_input *input)
{
	close_fd(input);
	while (input && input->type != PIPE)
	{
		if (input->type == HEREDOC)
			unlink(input->str);
		free(input->str);
		remove_head(&input);
	}
	if (input && input->type == PIPE)
		remove_head(&input);
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

void	executer(t_input *input)
{
	int		not_first_cmd;
	t_cmd	cmd;

	create_heredocs(input);
	not_first_cmd = -1;
	while (input)
	{
		not_first_cmd++;
		cmd.fd_in = STDIN_FILENO;
		cmd.fd_out = STDOUT_FILENO;
		open_block_files(input, &cmd);
		
		clean_block(input);
	}
}
