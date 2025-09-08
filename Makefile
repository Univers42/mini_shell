# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/09/08 13:08:20 by dlesieur          #+#    #+#              #
#    Updated: 2025/09/09 00:16:04 by dlesieur         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# **************************************************************************** #
# 							CONFIGURATION MAKEFILE                     		   #
# **************************************************************************** #

# COLOR VARIABLES
# --------------------
RESET		= \033[0m
BOLD		= \033[1m
UNDERLINE	= \033[4m
REVERSE		= \033[7m
BLACK		= \033[30m
RED			= \033[31m
GREEN		= \033[32m
YELLOW		= \033[33m
BLUE		= \033[34m
MAGENTA		= \033[35m
CYAN		= \033[36m
WHITE		= \033[37m
# --------------------


#CMD VARIABLES
# --------------------
NAME		= minishell
CC			= cc
CFLAGS		= -Wall -Wextra -Werror -MMD -MP -O3 -pedantic -std=c99
RM			= rm -rf
# --------------------


# MACROS UTILS
# --------------------
define compile_lib
	@echo "compiling library"
	$(MAKE) -C $(1) $(2)
endef
# --------------------


# **************************************************************************** #
#                                 Source files                                 #
# **************************************************************************** #

SRCS_DIR	= sources
SRCS		= $(shell find $(SRCS_DIR) -name *.c)

LIBFT_DIR	= $(INCLUDES_DIR)/libft
LIBFT_BIN	= $(LIBFT_DIR)/libft.a

LIBS		= $(LIBFT_BIN)

INCLUDES_DIR = includes
HEADERS		= $(shell find $(INCLUDES_DIR) -type f -name "*.h" -exec dirname {} \; | sort -u)
HEADERS		+= .
INCLUDES	= $(foreach dir,$(HEADERS),-I$(dir))
MAKEFLAGS += --no-print-directory
# **************************************************************************** #
#                                 Object files                                 #
# **************************************************************************** #

OBJS_DIR	= obj
OBJS 		= $(SRCS:$(SRCS_DIR)/%.c=$(OBJS_DIR)/%.o)

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.c
	@mkdir -p $(@D)
	@echo "Compiling $< -> $@"
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

-include $(OBJS:.o=.d)
# **************************************************************************** #
#                                    Rules                                     #
# **************************************************************************** #

all: $(NAME)

$(NAME): $(LIBFT_BIN) $(OBJS)
	@echo "$(BLUE)Linking objects to create $(NAME)$(RESET)"
	@echo "$(GREEN)Compilation successful!$(RESET)"
	@echo "$(YELLOW)./minishell\n./minishell -c \"command1 ; command2 ; ...$(RESET)"
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(LIBS) -lreadline

clean:
	$(RM) $(OBJS_DIR)

fclean: clean
	$(call compile_lib,$(LIBFT_DIR),fclean)
	$(RM) $(NAME)

re: fclean all

libft: $(LIBFT_BIN)

$(LIBFT_BIN):
	$(call compile_lib,$(LIBFT_DIR),all MLX_ENABLED=0)

help:
	@echo "Makefile rules:"
	@echo "  all     : Compile the project (default)"
	@echo "  clean   : Remove object files"
	@echo "  fclean  : Remove object files and the executable"
	@echo "  re      : Recompile the project (fclean + all)"
	@echo "  libft   : Compile the libft library"
	@echo "  help    : Display this help message"
	@echo "  norminette : Check code style with norminette"
	@echo ""
	@echo "Tip: Use 'make -j' or 'make -jN' to compile in parallel and speed up builds."

norminette:
	find . -path "./includes/libft/minilibx-linux" -prune -o -name "*.c" -exec norminette {} +

.PHONY: all clean fclean re libft
