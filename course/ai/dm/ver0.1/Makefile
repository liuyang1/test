CC		:=	g++
CFLAGS	:=	-Wall -O9

dm:main.o user.o userset.o action.o match.o train.o
	$(CC) $(CFLAGS) $^ -o $@
main.o:main.cpp
	$(CC) $(CFLAGS) -c $? -o $@
user.o:user.cpp
	$(CC) $(CFLAGS) -c $? -o $@
userset.o:userset.cpp
	$(CC) $(CFLAGS) -c $? -o $@
action.o:action.cpp
	$(CC) $(CFLAGS) -c $? -o $@
match.o:match.cpp
	$(CC) $(CFLAGS) -c $? -o $@
train.o:train.cpp
	$(CC) $(CFLAGS) -c $? -o $@

clean:
	rm -rf *.o dm
