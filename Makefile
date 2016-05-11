CC = clang++
CFLAGS = -g -std=c++11 -I .
EXEC = tictactoe
SRC = Tictactoe.cpp

DEPS = $(patsubst %.cpp, %.h, ${SRC})
# DEPS += 

OBJS = $(patsubst %.cpp, %.o, ${SRC})
OBJS += main.o

${EXEC}: ${OBJS}
	${CC} -o $@ $^ ${CFLAGS}

%.o: %.cpp ${DEPS}
	${CC} -o $@ -c $< ${CFLAGS}

clean:
	\rm -rf ${OBJS} ${EXEC} 
