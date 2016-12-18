
CXX    := g++ -std=c++11

OUTPUT := pstree.exe

all: ${OUTPUT}

SRCS = main.cpp

OBJS := $(addsuffix .o,$(basename ${SRCS}))

${OUTPUT}: ${OBJS}
	${LD} -o $@ ${LDFLAGS} ${OBJS}

%.o : %.cpp
	${CXX} -c $< -o $@

%.o : %.c
	${CC} -c $< -o $@

clean:
	-rm -f ${OBJS} ${OUTPUT}