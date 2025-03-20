/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_node.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samy_bravy <samy_bravy@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 14:24:28 by fgrisost          #+#    #+#             */
/*   Updated: 2024/08/31 10:06:08 by samy_bravy       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_input	*new_node(t_type type, char *str)
{
	t_input	*new_node;

	new_node = (t_input *)malloc(sizeof(t_input));
	new_node->type = type;
	if (str)
		new_node->str = ft_strndup(str, INT_MAX);
	else
		new_node->str = NULL;
	new_node->fd = -1;
	new_node->next = NULL;
	return (new_node);
}

static void	find_lst_cmd(t_input *temp, t_input **last_cmd)
{
	t_input	*save;

	save = temp;
	while ((temp))
	{
		if ((temp)->type == PIPE)
			(*last_cmd) = NULL;
		if ((temp)->type == CMD)
			(*last_cmd) = (temp);
		if (!(temp)->next)
			break ;
		(temp) = (temp)->next;
	}
	temp = save;
}

static void	concatenate_str(t_input *new_node, t_input *temp, t_input *last_cmd,
		char *temp_str)
{
	if (*(last_cmd->str) == '$')
		temp->next = new_node;
	else
	{
		temp_str = malloc(ft_strlen(last_cmd->str) + ft_strlen(new_node->str)
				+ 2);
		ft_strlcpy(temp_str, last_cmd->str, INT_MAX);
		ft_strlcat(temp_str, "\x1D", INT_MAX);
		ft_strlcat(temp_str, new_node->str, INT_MAX);
		free(last_cmd->str);
		last_cmd->str = temp_str;
		free(new_node->str);
		free(new_node);
	}
}

void	add_node(t_input **head, t_input *new_node)
{
	t_input	*temp;
	t_input	*last_cmd;
	char	*temp_str;

	temp_str = NULL;
	temp = *head;
	last_cmd = NULL;
	if (!(*head))
	{
		*head = new_node;
		return ;
	}
	find_lst_cmd(temp, &last_cmd);
	if (new_node->type == CMD && last_cmd)
		concatenate_str(new_node, temp, last_cmd, temp_str);
	else
	{
		while (temp->next)
			temp = temp->next;
		temp->next = new_node;
	}
}

t_type	identify_type(char *token)
{
	if (ft_strncmp(token, ">>", 2) == 0)
		return (APPEND);
	if (strncmp(token, "<<", 2) == 0)
		return (HEREDOC);
	if (ft_strncmp(token, ">", 1) == 0)
		return (TRUNC);
	if (strncmp(token, "<", 1) == 0)
		return (INPUT);
	if (strncmp(token, "|", 1) == 0)
		return (PIPE);
	return (CMD);
}
