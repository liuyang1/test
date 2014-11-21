ifneq ($(KERNELRELEASE), )
	obj-m		:=	testkiconv.o
else
	KERNELDIR	?=	/usr/src/linux
	PWD			:=	$(shell pwd)
default:
	$(MAKE)	 -C $(KERNELDIR) M=$(PWD) modules
endif

clean:
	rm *.o *.ko
