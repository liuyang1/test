CFLAGS += -Wall -Werror
# LDFALGS :=

SRCS := $(wildcard *.c)
OBJS := $(SRCS:.c=.o)

TARGET := main

$(TARGET): $(OBJS)
	gcc $(OBJS) $(LDFLAGS) -o $@

-include $(OBJS:.o=.d)

%.o: %.c
	gcc -c  $(CFLAGS) $*.c -o $*.o
	gcc -MM $(CFLAGS) $*.c >  $*.d

clean:
	rm -rf $(OBJS) $(TARGET) *.d core*

run: $(TARGET)
	./$(TARGET)

tag:
	gtags -i .

style:
	uncrustify -c $(HOME)/.uncrustify.cfg --no-backup $(SRCS)

check:
	cppcheck $(SRCS)
