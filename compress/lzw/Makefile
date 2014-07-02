FILE_SUFFIX	:= .c

SRCS		:= $(wildcard *$(FILE_SUFFIX))
OBJS		:= $(patsubst %$(FILE_SUFFIX),%.o,$(SRCS))

CFLAGS 		:= -Wall -std=c99
CC			:= gcc

com.out:com.o lzw.o
	$(CC) $^ -o $@
$(OBJS): %.o : %$(FILE_SUFFIX)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o com.out
