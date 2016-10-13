CFLAGS += -Wall -Werror

DFTSRCS = $(wildcard *.c)
DFTSRCS += $(wildcard *.cpp)
SRCS ?= $(DFTSRCS)

OBJS := $(SRCS:.cpp=.o)
OBJS := $(OBJS:.c=.o)

BUILDTYPE ?= c

TARGET ?= main

.PHONY: clean run stress tag style check doc view loc

SUFFIX=$(suffix $(TARGET))
# generate final file based on filetype of target
ifeq ($(BUILDTYPE), c)
ifeq ($(SUFFIX), .a)
$(TARGET): $(OBJS)
	ar rc $(TARGET) $(OBJS)
else ifeq ($(SUFFIX), .so)
CFLAGS+=-fPIC
$(TARGET): $(OBJS)
	gcc -shared -o $(TARGET) $(OBJS)
else
$(TARGET): $(OBJS)
	gcc $(OBJS) $(LDFLAGS) -o $@
endif

else ifeq ($(BUILDTYPE), c++)
$(TARGET): $(OBJS)
	g++ $(OBJS) $(LDFLAGS) -o $@
endif

-include $(OBJS:.o=.d)

%.o: %.c
	@echo "CC \t$*.c"
	@gcc -c  $(CFLAGS) $*.c -o $*.o
	@gcc -MM $(CFLAGS) $*.c >  $*.d

%.o: %.cpp
	@echo "C++ \t$*.cpp"
	@g++ -c $(CPPFLAGS) $*.cpp -o $*.o
	@g++ -MM $(CPPFLAGS) $*.cpp > $*.d

clean:
	rm -rf *.o *.d core* $(TARGET)
	rm -rf html latex
	rm -rf .cccc

run: $(TARGET)
	./$(TARGET)

# loop run TARGET program, until it fail
stress: $(TARGET)
	cnt=0 ; while true; do \
		cnt=`echo $$cnt + 1 | bc`; \
		tput setf 2; echo "---- test $$cnt times ----"; tput setf 7;\
		./$(TARGET) || exit 1; \
		sleep 1; \
	done

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

loc:
	cloc .
