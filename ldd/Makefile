#obj-m := hello.o
ifneq ($(KERNELRELEASE), )
	obj-m		:=	hello.o
else
	KERNELDIR	?=	/usr/src/linux
	PWD			:=	$(shell pwd)
default:
	$(MAKE)	-C $(KERNELDIR) M=$(PWD) modules
endif

#clean:
#	rm *.o *.ko
