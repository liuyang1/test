RES=result.txt
test: show_bytes.c
	gcc show_bytes.c
	uname -a >> ${RES}
	./a.out >> ${RES}
	echo "----" >> ${RES}

clean:
	rm -rf a.out

reset:
	rm -rf ${RES}

show:
	cat ${RES}
