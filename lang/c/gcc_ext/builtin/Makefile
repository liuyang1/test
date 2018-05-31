all: builtin builtin.asm

builtin: builtin.c
	gcc builtin.c -o builtin

builtin.asm: builtin
	objdump -d builtin > builtin.asm

clean:
	rm -rf builtin builtin.asm
