src = $(wildcard *.c)
obj = $(src:.c=.o)
dep = $(obj:.o=.d)

$(info $(src))

hello: $(obj)
	$(CC) $^ -o $@

dep: ${dep}

%.d: %.c
	$(CPP) $< -MM -MT $(@:.d=.o) > $@

# supress error even dep don't exist
-include ${dep}

clean: cleandep
	rm -rf hello ${obj}

cleandep:
	rm -rf ${dep}
