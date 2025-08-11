NAME=minishell
CC=cc
CFLAGS=-Wall -Wextra -Werror -g3 -O3
PROJECT_DIR = $(shell pwd)
SRC_DIR = $(PROJECT_DIR)/sources
INCLUDE_DIR = $(PROJECT_DIR)/include
SRCS=$(shell find $(SRC_DIR) -type f -name "*.c")
OBJS = $(patsubst $(SRC_DIR)/%, $(OBJ_DIR)/%, $(SRCS:.c=.o))

LIBFT_DIR= $(INCLUDE_DIR)/libft
LIBFT_A = $(LIBFT_DIR)/libft.a
OBJ_DIR= $(PROJECT_DIR)/object

AR=ar rcs

all: $(NAME)

$(NAME): $(OBJS)
	$(AR) $(OBJS) $(NAME)

%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@




	
