# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/11/25 17:53:15 by ashishae          #+#    #+#              #
#    Updated: 2020/07/11 16:14:27 by ashishae         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRCS = srcs/main.c srcs/libft_lite.c srcs/structure_management.c \
srcs/utilities.c srcs/philosopher.c srcs/monitoring.c srcs/threading.c \
srcs/print_long.c srcs/main_process.c
OBJS = $(SRCS:.c=.o)
NAME = philo_three

CFLAGS = -Wall -Wextra -Werror -pthread #-fsanitize=address

.PHONY: 	clean fclean all re bonus

all:		$(NAME)

.c.o:
			gcc $(CFLAGS) -c $< -o $@

$(NAME):	$(OBJS)
			gcc -o $(NAME) $(OBJS) $(CFLAGS)

clean:
			rm -f $(OBJS)

fclean: 	clean
			rm -f $(NAME)

re:			fclean all
