# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jeberle <jeberle@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/05/25 22:43:08 by jonathanebe       #+#    #+#              #
#    Updated: 2024/06/21 15:34:20 by jeberle          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#------------------------------------------------------------------------------#
#--------------                       PRINT                       -------------#
#------------------------------------------------------------------------------#

RED := \033[31m
GREEN := \033[32m
YELLOW := \033[33m
BLUE := \033[34m
MAGENTA := \033[35m
CYAN := \033[36m
X := \033[0m

SUCCESS := \n$(BLUE)\
███████████████████████████████████████████████████████████████████$(X)\n\
$(X)\n\
███ ███  █  ██    █  █  ███████  █     █  ███████  █        █      $(X)\n\
█  █  █  █  █ █   █  █  █        █     █  █        █        █      $(X)\n\
█  █  █  █  █  █  █  █  ███████  ███████  ███████  █        █      $(X)\n\
█     █  █  █   █ █  █        █  █     █  █        █        █      $(X)\n\
█     █  █  █    ██  █  ███████  █     █  ███████  ███████  ███████$(X)\n\
$(X)\n\
$(BLUE)███████████████████████████████████████████████████████████████████$(X)\n\

#------------------------------------------------------------------------------#
#--------------                      GENERAL                      -------------#
#------------------------------------------------------------------------------#

NAME=minishell
NAME_BONUS=minishell_bonus

#------------------------------------------------------------------------------#
#--------------                       FLAGS                       -------------#
#------------------------------------------------------------------------------#

CC=cc
CFLAGS=-Wall -Wextra -Werror

ifeq ($(DEBUG), 1)
	CFLAGS += -fsanitize=address -g
endif

DEPFLAGS=-MMD -MP

#------------------------------------------------------------------------------#
#--------------                        DIR                        -------------#
#------------------------------------------------------------------------------#

OBJ_DIR := ./obj
DEP_DIR := $(OBJ_DIR)/.deps
INC_DIRS := .
SRC_DIRS := .

vpath %.c $(SRC_DIRS)
vpath %.h $(INC_DIRS)
vpath %.d $(DEP_DIR)

#------------------------------------------------------------------------------#
#--------------                        LIBS                       -------------#
#------------------------------------------------------------------------------#

LIBFT_DIR=libft
LIBFT=libft.a
LIBFT_LIB=$(LIBFT_DIR)/$(LIBFT)
LIBFTFLAGS=-L$(LIBFT_DIR) -lft

SYSLIBFLAGS=-lreadline

#------------------------------------------------------------------------------#
#--------------                        SRC                        -------------#
#------------------------------------------------------------------------------#

SRCS=	mandatory/ast.c \
		mandatory/hierarchy_validation.c \
		mandatory/lexer.c \
		mandatory/init_envlst.c \
		mandatory/minishell.c \
		mandatory/parser.c \
		mandatory/putters.c \
		mandatory/segments_helper.c \
		mandatory/segments.c \
		mandatory/tokens.c \
		builtins/cd.c \
		builtins/echo.c \
		builtins/env.c \
		builtins/exit.c \
		builtins/export.c \
		builtins/pwd.c \
		builtins/unset.c \

BONUS_SRCS= \
# bonus/minishell_bonus.c \

#------------------------------------------------------------------------------#
#--------------                      OBJECTS                      -------------#
#------------------------------------------------------------------------------#

OBJECTS := $(addprefix $(OBJ_DIR)/, $(SRCS:%.c=%.o))
BONUS_OBJECTS := $(addprefix $(OBJ_DIR)/, $(BONUS_SRCS:%.c=%.o))

#------------------------------------------------------------------------------#
#--------------                      COMPILE                      -------------#
#------------------------------------------------------------------------------#

.PHONY: all clean fclean re libft

all: $(LIBFT_LIB) $(NAME)

bonus: $(LIBFT_LIB) $(NAME_BONUS)

-include $(OBJECTS:.o=.d)
-include $(BONUS_OBJECTS:.o=.d)

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) $(DEPFLAGS) -c $< -o $@

$(LIBFT_LIB):
	@git submodule update --init --recursive --remote > /dev/null 2>&1
	@$(MAKE) -C $(LIBFT_DIR)

$(NAME): $(OBJECTS)
	@$(CC) -o $@ $^ $(LIBFTFLAGS) $(SYSLIBFLAGS)
	@echo "$(SUCCESS)"

$(NAME_BONUS): $(BONUS_OBJECTS)
	@$(CC) -o $@ $^ $(LIBFTFLAGS) $(SYSLIBFLAGS)
	@echo "$(SUCCESS)"

clean:
	@rm -rf $(OBJ_DIR)
	@$(MAKE) -C $(LIBFT_DIR) clean
	@echo "$(RED)objects deleted$(X)"

fclean: clean
	@rm -rf $(NAME_BONUS)
	@rm -rf $(NAME)
	@$(MAKE) -C $(LIBFT_DIR) fclean
	@echo "$(RED)minishell deleted$(X)"

re: fclean all
