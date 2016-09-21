# CFLAGS :=
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
	rm -rf $(OBJS) *.d $(TARGET)

run:
	./$(TARGET)
