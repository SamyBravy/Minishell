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
	// cat < input.txt | grep test | /bin/cat > output.txt | wc -l << END | ./test_funzioni/a.out

	t_input *input = malloc(sizeof(t_input));
	input->type = CMD;
	input->str = ft_strdup("cast");
	input->next = NULL;
	input->fd = -1;

	t_input *input1 = malloc(sizeof(t_input));
	input1->type = INPUT;
	input1->str = ft_strdup("input.txt");
	input1->next = NULL;
	input1->fd = -1;

	t_input *input2 = malloc(sizeof(t_input));
	input2->type = PIPE;
	input2->str = NULL;
	input2->next = NULL;
	input2->fd = -1;

	t_input *input3 = malloc(sizeof(t_input));
	input3->type = CMD;
	input3->str = ft_strdup("grep test");
	input3->next = NULL;
	input3->fd = -1;

	t_input *input4 = malloc(sizeof(t_input));
	input4->type = PIPE;
	input4->str = NULL;
	input4->next = NULL;
	input4->fd = -1;

	t_input *input5 = malloc(sizeof(t_input));
	input5->type = CMD;
	input5->str = ft_strdup("/bin/cat");
	input5->next = NULL;
	input5->fd = -1;

	t_input *input6 = malloc(sizeof(t_input));
	input6->type = TRUNC;
	input6->str = ft_strdup("output.txt");
	input6->next = NULL;
	input6->fd = -1;

	t_input *input7 = malloc(sizeof(t_input));
	input7->type = PIPE;
	input7->str = NULL;
	input7->next = NULL;
	input7->fd = -1;

	t_input *input8 = malloc(sizeof(t_input));
	input8->type = CMD;
	input8->str = ft_strdup("wc -l");
	input8->next = NULL;
	input8->fd = -1;

	t_input *input9 = malloc(sizeof(t_input));
	input9->type = HEREDOC;
	input9->str = ft_strdup("END");
	input9->next = NULL;
	input9->fd = -1;

	t_input *input10 = malloc(sizeof(t_input));
	input10->type = PIPE;
	input10->str = NULL;
	input10->next = NULL;
	input10->fd = -1;

	t_input *input11 = malloc(sizeof(t_input));
	input11->type = CMD;
	input11->str = ft_strdup("./test_funzioni/a.out");
	input11->next = NULL;
	input11->fd = -1;

	// Creazione della lista collegata
	input->next = input1;
	input1->next = input2;
	input2->next = input3;
	input3->next = input4;
	input4->next = input5;
	input5->next = input6;
	input6->next = input7;
	input7->next = input8;
	input8->next = input9;
	input9->next = input10;
	input10->next = input11;

	// Creazione e scrittura su file di input
	FILE *file = fopen("input.txt", "w");
	fprintf(file, "This is a test input\n file.\n");
	fclose(file);

	// Esecuzione del comando e gestione dello stato di uscita
	int exit_status = 0;
	executer(input, env, &exit_status);

	// Pulizia dei file temporanei
	unlink("input.txt");
	unlink("output.txt");

	(void) argc;
	(void) argv;
	(void) env;
	return (0);
}
