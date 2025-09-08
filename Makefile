# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: sede-san <sede-san@student.42madrid.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/09/08 13:08:20 by dlesieur          #+#    #+#              #
#    Updated: 2025/09/08 17:15:59 by sede-san         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME			= minishell
LIBFT_DIR		= $(INCLUDES_DIR)/libft
INCLUDES_DIR	= includes

CC			= cc
CFLAGS		= -Wall -Wextra -Werror
RM			= rm -rf
INCLUDES	= -I $(INCLUDES_DIR) -I $(LIBFT_DIR)

define compile_lib
	@echo "compiling library"
	$(MAKE) -C $(1) $(2)
endef

# **************************************************************************** #
#                                 Source files                                 #
# **************************************************************************** #

SRCS_DIR	= sources
# \/ ! Add explicit files before push to main
SRCS		= $(shell find $(SRCS_DIR) -name *.c)

# **************************************************************************** #
#                                 Object files                                 #
# **************************************************************************** #

OBJS_DIR	= obj
OBJS 		= $(SRCS:$(SRCS_DIR)/%.c=$(OBJS_DIR)/%.o)

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.c
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# **************************************************************************** #
#                                    Rules                                     #
# **************************************************************************** #

all: clib $(NAME)
.PHONY: all

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $(NAME)

clean:
	$(RM) $(OBJS_DIR)
.PHONY: clean

fclean: clean
	$(RM) $(NAME)
.PHONY: fclean

re: fclean all
.PHONY: re

clib: libft
.PHONY: clib

LIBFT_BIN	= $(LIBFT_DIR)/libft.a
libft: $(LIBFT_BIN)
.PHONY: libft

$(LIBFT_BIN):
	$(call compile_lib,$(LIBFT_DIR),all MLX_ENABLED=0)
