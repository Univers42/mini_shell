##########################  CONFIGURATION VARIABLES

NAME=minishell
CC=cc
CFLAGS = -Wall -Wextra -Werror
CFLAGS += -g3 -O3
PROJECT_DIR = $(shell pwd)
SRC_DIR = $(PROJECT_DIR)/srcs
INCLUDE_DIR = $(PROJECT_DIR)/inc

#########################
# This section is here to look dynamically all archives *.c in any directories
# of the implementation from the srcs dir
# and redirect all the objects files into an OBJ dir
# also we would look for all headers files up to our library depth because
# the library handle dynamically the search of files *.h
# as the design of my library is alrady modularized into different categries
# or in other words each directories as its own *.h implementation
# it generates isues with the compilator that's not able to find dynamically the
# header files.
###########################
SRCS=$(shell find $(SRC_DIR) -type f -name "*.c")
OBJS = $(patsubst $(SRC_DIR)/%, $(OBJ_DIR)/%, $(SRCS:.c=.o))

LIBFT_DIR= $(INCLUDE_DIR)/libft
LIBFT_A = $(LIBFT_DIR)/libft.a

OBJ_DIR= $(PROJECT_DIR)/object

HEADER_DIRS := $(shell find $(INCLUDE_DIR) -type f -name "*.h" -exec dirname {} \; sort -u)
INCLUDE_FLAGS := $(foreach dir, $(HEADER_DIRS),-I$(dir))

all: $(NAME)

$(NAME): $(LIBFT_A) $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LIBFT_A) -lreadline

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(LIBFT_A):
	$(MAKE) -C $(LIBFT_DIR) all

norminette :
	norminette

clean:
	rm -f $(OBJS)
	rm -rf $(OBJ_DIR)

fclean: clean
	$(MAKE) -C $(LIBFT_DIR) fclean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re norminette
