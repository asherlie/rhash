CC=gcc
CFLAGS= -Wall -Wextra -Wpedantic -Werror -O3

all: rh

rh: rh.c

.PHONY:
clean:
	rm -f rh
