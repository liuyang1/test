FILE_SUFFIX	:= .c

SRCS		:= $(wildcard *$(FILE_SUFFIX))
OBJS		:= $(patsubst %$(FILE_SUFFIX),%.o,$(SRCS))

CFLAGS 		:= -Wall -std=c99
CC			:= gcc

all:Columbus.out

Columbus.out:Columbus.o
	$(CC) $^ -o $@
$(OBJS): %.o : %$(FILE_SUFFIX)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o *.out
