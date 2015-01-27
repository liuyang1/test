# ABC = "abc"
# ifeq ($(ABC), )
# warn_ABC:
# 	echo "ABCnot exist"
# endif
#
.PHONY:all bg2q
all:bg2q
	echo "all done"
bg2q: DEF=def
bg2q: ABC=abc_bg2q
bg2q: default
	echo "bg2q $(ABC) $(DEF)"
	echo "bg2q done"
default:
	echo "default bg2q $(ABC) $(DEF)"
	echo "default"
