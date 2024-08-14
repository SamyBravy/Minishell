/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sdell-er <sdell-er@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 15:34:57 by sdell-er          #+#    #+#             */
/*   Updated: 2024/08/11 18:01:33 by sdell-er         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char **argv, char **env)
{
	t_input *input = malloc(sizeof(t_input));
	input->type = CMD;
	input->str = ft_strdup("pwd casdfjsf dsfjdsf");
	input->next = NULL;
	input->fd = -1;

	t_input *input2 = malloc(sizeof(t_input));
	input2->type = PIPE;
	input2->str = NULL;
	input2->next = NULL;
	input2->fd = -1;

	t_input *input3 = malloc(sizeof(t_input));
	input3->type = CMD;
	input3->str = ft_strdup("cat -e");
	input3->next = NULL;
	input3->fd = -1;

	input->next = input2;
	input2->next = input3;

	// Creazione e scrittura su file di input
	FILE *file = fopen("input.txt", "w");
	fprintf(file, "This is a test input\n file.\n");
	fclose(file);

	// Esecuzione del comando e gestione dello stato di uscita
	int exit_status = 0;
	executer(&input, env, &exit_status);
	printf("Exit status: %d\n", exit_status);

	// Pulizia dei file temporanei
	unlink("input.txt");
	unlink("output.txt");

	(void) argc;
	(void) argv;
	(void) env;
	return (0);
}
