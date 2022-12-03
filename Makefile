NAME = ircserv
SRCS = srcs/main.cpp srcs/MyServer.cpp srcs/errors_msg.cpp srcs/Clients.cpp srcs/MyMsg.cpp \
srcs/Channels.cpp

INCLUDE= irc.hpp

OBJS = $(SRCS:.cpp=.o)
DEPS = $(SRCS:.cpp=.d)

CC= c++
FLAGS= -Wall -Wextra -Werror -std=c++98 -g3

%.o:				%.cpp
	@$(CC) $(FLAGS) -MMD -c $< -o $@ -I $(INCLUDE)

all: $(NAME)

$(NAME): $(OBJS)
	@echo "Compilation du programme $(NAME)"
	$(CC) $(FLAGS) $(OBJS) -o $(NAME)

clean:
	rm -Rf $(OBJS) $(DEPS)

fclean: clean
	rm -Rf $(NAME)

re: fclean all

-include $(DEPS)
.PHONY: all clean fclean re