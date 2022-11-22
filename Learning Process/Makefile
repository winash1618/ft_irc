NAME = ft_irc
CFLAGS = -Wall -Wextra -Werror
CC = c++
SRC = main.cpp Message.cpp Server.cpp Channel.cpp User.cpp
OBJ = ${SRC:.c=.o}

all: ${NAME}

${NAME}: ${OBJ}
	${CC} ${CFLAGS} ${OBJ} -o $@

%.o: %.c