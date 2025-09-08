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
CC			= cc
CFLAGS		= -Wall -Wextra -Werror
RM			= rm -rf

define compile_lib
	@echo "compiling library"
	$(MAKE) -C $(1) $(2)
endef

# **************************************************************************** #
#                                 Source files                                 #
# **************************************************************************** #

SRCS_DIR	:= sources
SRCS		:= $(shell find $(SRCS_DIR) -name *.c)

LIBFT_DIR	= $(INCLUDES_DIR)/libft
LIBFT_BIN	:= $(LIBFT_DIR)/libft.a

INCLUDES_DIR = includes
HEADERS		= $(shell find $(INCLUDES_DIR) -type f -name "*.h" -exec dirname {} \; | sort -u)
HEADERS		+= .
INCLUDES	= $(foreach dir,$(HEADERS),-I$(dir))

# **************************************************************************** #
#                                 Object files                                 #
# **************************************************************************** #

OBJS_DIR	:= obj
OBJS 		:= $(SRCS:$(SRCS_DIR)/%.c=$(OBJS_DIR)/%.o)

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.c
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# **************************************************************************** #
#                                    Rules                                     #
# **************************************************************************** #

all: $(NAME)

$(NAME): clib $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $(NAME)

clean:
	$(RM) $(OBJS_DIR)

fclean: clean
	$(call compile_lib,$(LIBFT_DIR),fclean)
	$(RM) $(NAME)

re: fclean all

clib: libft

libft: $(LIBFT_BIN)

$(LIBFT_BIN):
	$(call compile_lib,$(LIBFT_DIR),all MLX_ENABLED=0)

.PHONY: all clean fclean re clib libft 
