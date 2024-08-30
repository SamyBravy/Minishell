/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove_empty.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgrisost <fgrisost@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 14:39:13 by fgrisost          #+#    #+#             */
/*   Updated: 2024/08/30 14:59:19 by fgrisost         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	remove_empty_nodes_help(t_input **current, t_input **prev)
{
	*prev = *current;
	*current = (*current)->next;
}

void	remove_empty_nodes(t_input **head)
{
	t_input	*current;
	t_input	*prev;
	t_input	*temp;

	current = *head;
	prev = NULL;
	while (current != NULL)
	{
		if (current->type == CMD)
			current->str = remove_spaces(current->str);
		if (current->str != NULL && (current->str[0] == '\0'
				|| just_spaces(current->str) == 1))
		{
			temp = current;
			if (prev == NULL)
				*head = current->next;
			else
				prev->next = current->next;
			current = current->next;
			free(temp->str);
			free(temp);
		}
		else
			remove_empty_nodes_help(&current, &prev);
	}
}