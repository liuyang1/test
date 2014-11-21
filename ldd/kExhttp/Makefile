ifneq ($(KERNELRELEASE), )
	# modules name 
	# Don't duplicate name with files under same dir
	MODULE_NAME		:=	tconv
	# core objs (cotain init&exit method)
	RESMAIN_CORE_OBJS	:=	core.o
	# dep file
	RESMAIN_GLUE_OBJS	:=	conv.o exhttp.o
	# all obj files
	$(MODULE_NAME)-objs :=	$(RESMAIN_GLUE_OBJS) $(RESMAIN_CORE_OBJS)
	# final obj file link to kernel obj
	obj-m				:=	tconv.o
else
	KERNELDIR			?=	/usr/src/linux
	PWD					:=	$(shell pwd)
modules:
	$(MAKE)	 -C $(KERNELDIR) M=$(PWD) modules
modules_install:
	$(MAKE)  -C $(KERNELDIR) M=$(PWD) modules_install
clean:
	rm -rf *.o *~ core .depend .*.cmd *.ko *.mod.c .tmp_versions modules.order module.symvers
.PHONY:modules modules_install clean

endif
