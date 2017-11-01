# How to pass var from main.mak to sub makefile
# ref: https://stackoverflow.com/questions/12697012/passing-variables-from-include-directive-to-sub-make
env:=liuyang
shadow:=shadow

# use export
export env

all:
	@echo "env in main.mak:" $(env)
	@echo "shadow in main.mak:" $(shadow)
	@make -f sub.mak
