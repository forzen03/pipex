CC = cc
CFLAGS = -Wall -Wextra -Werror
NAME = pipex
NAME_BONUS = pipex_bonus
SRC = pipex.c pipex_utils.c pipex_utils2.c
SRC_BONUS = pipex_bonus.c pipex_utils_bonus.c pipex_utils2_bonus.c first_process_bonus.c \
			mid_process_bonus.c last_process_bonus.c here_doc_bonus.c 
OBJ = $(SRC:.c=.o)
OBJ_BONUS = $(SRC_BONUS:.c=.o)
LIBFT_DIR = ./libft

all : $(NAME)

bonus : $(NAME_BONUS)


$(NAME): $(OBJ)
	$(MAKE) -C $(LIBFT_DIR)
	$(CC) $(CFLAGS) $(OBJ) -L$(LIBFT_DIR) -lft -o $(NAME)

$(NAME_BONUS) : $(OBJ_BONUS)
		$(MAKE) -C $(LIBFT_DIR)
		$(CC) $(CFLAGS) $(OBJ_BONUS) -L$(LIBFT_DIR) -lft -o $(NAME_BONUS)

%.o:%.c 
	$(CC) $(CFLAGS) -c $< -o $@

clean :
	$(MAKE) -C $(LIBFT_DIR) clean
	rm -f $(OBJ) $(OBJ_BONUS)

fclean : clean
	$(MAKE) -C $(LIBFT_DIR) fclean
	rm -f $(NAME) $(NAME_BONUS)

re : fclean all

.PHONY : all clean fclean re bonus