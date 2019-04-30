all: orig wrap
	./orig
	./wrap

FLAGS = -Wall -g

# modified flags only for WRAP target
wrap : FLAGS += -Wl,--wrap=printf,--wrap=malloc -fno-builtin -DWRAP

orig: wrap.c
	gcc $(FLAGS) $^ -o $@

wrap: wrap.c
	gcc $(FLAGS) $^ -o $@
	objdump -d wrap > wrap.asm

clean:
	rm -rf orig wrap wrap.asm
