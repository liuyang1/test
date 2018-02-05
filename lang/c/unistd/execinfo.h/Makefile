a.out: backtrace.c
	gcc -rdynamic backtrace.c

# use -rdynamic option to enable symbol
# -rdynamic
#  Pass the flag -export-dynamic to the ELF linker, on targets that support it.
#  This instructs the linker to add all symbols, not only used ones,
#      to the dynamic symbol table.
#  This option is needed for some uses of dlopen or to allow obtaining
#      backtraces from within a program.

clean:
	rm -rf a.out

run:
	./a.out
