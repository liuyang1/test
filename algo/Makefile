TARGET		:= shuffle
OUTDIR		:= .
NVERSION	:= 0.1
INCLUDES	:= 
LIBS		:= -lm
FILE_SUFFIX	:= .c
CFLAGS      +=  -Wall -std=c99#-DUSING_RTC
#DCFLAGS	:= $(DCFLAGS_PASS)
DCFLAGS         += #-DUSING_RTC
#LDFLAG		:= $(LDFLAGS_PASS)
#DLDFLAGS	:= $(DLDFLAGS_PASS)
CCFLAGS 	+= -O3 -Wall -std=c99
CC		:= gcc
#CXX		:= c++
#release	:= 1

#OBJS=ac3.o nal.o reader.o main.o
#SRCS 		:= $(wildcard *$(FILE_SUFFIX))
SRCS 		:= $(TARGET)$(FILE_SUFFIX) #$(wildcard ../*$(FILE_SUFFIX))
OBJS 		:= $(patsubst %$(FILE_SUFFIX),%.o,$(SRCS))
$(TARGET):$(OBJS)
	$(CC) $^ -o $@ $(LIBS)
$(OBJS): %.o : %$(FILE_SUFFIX)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@ 

.PHONY:run clean edit
edit:
	vim ./$(SRCS)
run:
	./$(TARGET)
clean:
	-rm -f $(OBJS) ./$(TARGET) *.o
