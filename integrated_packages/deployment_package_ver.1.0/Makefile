# Generic Makefile for Wt "witty" programs on the CS server
# 02/28/24, jajerkins@una.edu
#
# "make start" will compile (if needed) and launch your program
# "make stop" will terminate your program, ALWAYS stop after you've tested
#
PROG= AASS		# set your program name here
PORT= 8081			# any port in the range 8080-8085 is valid
SRCS!= ls *.cpp
OBJS= ${SRCS:S/cpp$/o/g}

CXXFLAGS=  -Wall -std=c++17
CXXFLAGS+= -I/usr/local/include
LDFLAGS=  -L/usr/local/lib
LDFLAGS+= -lwt -lwthttp -l:libwtdbo.so.4.10.3 -l:libwtdbosqlite3.so.4.10.3
LDFLAGS+= -l:libwtdbomysql.so.4.10.3

.PHONY: all clean start stop

all: ${PROG}

${PROG}: ${OBJS}
	${CXX} ${CXXFLAGS} -o $@ ${OBJS} ${LDFLAGS}

clean:
	@rm -f ${PROG} ${OBJS} resources

start: ${PROG}
	@ln -fs /usr/local/share/Wt/resources ./resources
	./${PROG} --docroot ./ --http-address 0.0.0.0 --http-port ${PORT} &

stop:
	@rm -f resources
	@pkill ${PROG}

run: ${PROG}
	@ln -fns /usr/local/share/Wt/resources ./resources
	./${PROG} --docroot ./ --http-address 0.0.0.0 --http-port ${PORT}


