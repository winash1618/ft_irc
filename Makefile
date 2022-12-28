NAME = ircserv
CPPFLAGS = -Wall -Wextra -Werror -std=c++98
CC = c++
SRC = main.cpp Message.cpp Server.cpp Channel.cpp User.cpp
OBJ = $(SRC:.cpp=.o)

all: ${NAME}

${NAME}: ${OBJ}
	${CC} ${CPPFLAGS} ${OBJ} -o $@

clean:
	rm -rf ${OBJ}

fclean:	clean
	rm -rf ${NAME}

re: fclean all