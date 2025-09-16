# -=-=-=-=-    NAME -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= #

NAME		:= 	ft_ping

# -=-=-=-=-    PATH -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= #

INCS		:=	incs

# -=-=-=-=-    FILES -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- #

SRC 		:= 	main.c						\
				parse.c						\
				socket.c					\
				send_ping.c

SRCDIR		= srcs
SRCS		= $(addprefix $(SRCDIR)/, $(SRC))

OBJDIR		= .obj
OBJS		= $(addprefix $(OBJDIR)/, $(SRC:.c=.o))

DEPDIR		= .dep
DEPS		= $(addprefix $(DEPDIR)/, $(SRC:.c=.d))
DEPDIRS		= $(DEPDIR)

HEADER		:=	incs/ft_ping.h

MAKE		:=	Makefile

# -=-=-=-=-    FLAGS -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= #

CC			:=	cc

CFLAGS		:=	-Werror -Wextra -Wall -g -fsanitize=address

DFLAGS		:= 	-MT $@ -MMD -MP

INCLUDE		:=	-I/incs

RM			:=	/bin/rm -fr

# -=-=-=-=-    TARGETS -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- #

all: $(NAME)

# -=-=-=-=-    RULES -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= #

$(OBJDIR)/%.o: $(SRCDIR)/%.c Makefile
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) $(INCLUDE) -MT $@ -MMD -MP -c $< -o $@
	@mkdir -p $(DEPDIR)
	@mv $(patsubst %.o,%.d,$@) $(subst $(OBJDIR),$(DEPDIR),$(@D))/

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) $(INCLUDE) $(LIBS) -o $(NAME) -lm
# TO BE DELETED WHEN DONE TESTING
	@sudo chown root $(NAME)
	@sudo chmod u+s $(NAME)

clean:
	@$(RM) $(OBJDIR) $(DEPDIR)

fclean: clean
	@$(RM) $(NAME)

re: fclean all

-include $(DEPS)

.PHONY:  all clean fclean re