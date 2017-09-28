str=abc def
# str=
num:=$(words $(str))

# first argument to 'word' function must be greater than 0
# avoid this error
ifneq (${num},0)
abc:=$(word $(num),$(str))
endif

main:
	echo $(num)
	echo $(abc)
