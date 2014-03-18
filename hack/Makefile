CFLAGS=-fno-stack-protector -g -Wall
all:vuln exploit

vuln:vuln.c
	gcc ${CFLAGS} $< -o $@
exploit:exploit.c
	gcc ${CFLAGS} $< -o $@

clean:
	rm vuln exploit
