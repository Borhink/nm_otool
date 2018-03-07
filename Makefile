# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: qhonore <qhonore@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2015/11/24 10:30:43 by qhonore           #+#    #+#              #
#    Updated: 2018/03/07 16:15:48 by qhonore          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRC_PATH = ./src/
OBJ_PATH = ./obj/
INC_PATH = ./inc/

SRC_NAME = nm.c libft.c libft2.c handle_64.c print_64.c handle_32.c print_32.c\
		   handle_fat.c
OBJ_NAME = $(SRC_NAME:.c=.o)
NAME = ft_nm

SRC_NAME2 = otool.c
OBJ_NAME2 = $(SRC_NAME2:.c=.o)
NAME2 = ft_otool

SRC = $(addprefix $(SRC_PATH),$(SRC_NAME))
OBJ = $(addprefix $(OBJ_PATH),$(OBJ_NAME))

SRC2 = $(addprefix $(SRC_PATH),$(SRC_NAME2))
OBJ2 = $(addprefix $(OBJ_PATH),$(OBJ_NAME2))

INC = $(addprefix -I,$(INC_PATH))

CC = gcc
CFLAGS = -MMD -Wall -Wextra -Werror

all:
	@echo "\033[32;44m Make $(NAME) \033[0m"
	@make $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@

$(NAME2): $(OBJ2)
	$(CC) $(CFLAGS) $^ -o $@

$(OBJ_PATH)%.o: $(SRC_PATH)%.c
	@mkdir -p $(OBJ_PATH)
	$(CC) $(CFLAGS) $(INC) -o $@ -c $<

clean:
	@echo "\033[31;44m Make clean $(NAME) & $(NAME2) \033[0m"
	rm -rf $(OBJ) $(OBJ2) $(OBJ_PATH)

fclean: clean
	@echo "\033[31;44m Make fclean $(NAME) & $(NAME2) \033[0m"
	rm -f $(NAME) $(NAME2)

re: fclean all

-include $(OBJ:.o=.d)
-include $(OBJ2:.o=.d)

.PHONY: all clean fclean re
