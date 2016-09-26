CFLAGS += -Wall -Werror
# LDFALGS :=

SRCS ?= $(wildcard *.c)
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
	rm -rf html latex
	rm -rf .cccc

run: $(TARGET)
	./$(TARGET)

stress: $(TARGET)
	while true; do ./$(TARGET); sleep 1; done

tag:
	gtags -i .

style:
	uncrustify -c $(HOME)/.uncrustify.cfg --no-backup $(SRCS)

check:
	cppcheck $(SRCS)

doc:
	doxygen .doxygen.cfg
	cccc $(SRCS)

view:
	python2 -m SimpleHTTPServer
