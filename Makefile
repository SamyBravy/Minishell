# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: sdell-er <sdell-er@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/01/16 19:17:18 by sdell-er          #+#    #+#              #
#    Updated: 2024/08/11 15:17:44 by sdell-er         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# "" e '' quando non sono attaccati a qualcosa sono sostituiti da \x1E (se sono attaccati solo tra loro si sostituisce tutto con \x1E)
# tutti gli fd della struttura sono initializzati a -1
# l'unica roba allocata che ci sarà quando viene chiamato l'executer sarà la lista passata all'executer
# si alloca una stringa per ogni nodo della lista, tranne nel caso in cui type è PIPE, in quel caso str = NULL
# valgrind --leak-check=full --show-leak-kinds=all --suppressions=readline.supp --trace-children=yes ./minishell
NAME = minishell
MY_LIB = ./my_lib
SRC = 	main.c \
		\
		executer/executer.c executer/clean.c executer/handle_signals.c \
		executer/get_cmd_info.c executer/ft_heredocs.c executer/execute_command.c \
		\
		builtins/exit_builtin.c builtins/echo_builtin.c
FLAGS = -g -Wall -Wextra -Werror -lreadline
CC = cc

all: $(NAME)

$(NAME): $(SRC)
	@make -C $(MY_LIB) 2>&1 | grep -v 'Leaving directory' | grep -v 'Entering directory'
	@$(CC) $(SRC) -o $(NAME) -L$(MY_LIB) -lmy_lib $(FLAGS)
	@echo "\e[0;93m[$(NAME)] compiled!\e[0m"

clean:
	@make clean -C $(MY_LIB) 2>&1 | grep -v 'Entering directory' | grep -v 'Leaving directory'

fclean:
	@make fclean -C $(MY_LIB) 2>&1 | grep -v 'Entering directory' | grep -v 'Leaving directory'
	@rm -f $(NAME)
	@echo "\e[0;91m[$(NAME)] deleted!\e[0m"

re: fclean all

.PHONY: all clean fclean re
