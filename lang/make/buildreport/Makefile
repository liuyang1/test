OBJS=main.o
main: main.o one.o
	gcc -o $@ $^

$(OBJS): main.c
	$(CC) $(MODULE_CFLAGS) -o $@ -c $< 2>&1 | tee -a buildreport
one.o: one.c
	$(CC) $(MODULE_CFLAGS) -o $@ -c $< 2>&1 | tee -a buildreport


clean:
	rm -rf *.o buildreport
