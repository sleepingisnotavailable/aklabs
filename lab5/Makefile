ifneq ($(KERNELRELEASE),)
	# kbuild part of makefile
	obj-m := hello5.o
	ccflags-y += -g -DDEBUG
else
	# normal makefile
	KDIR ?= /lib/modules/`uname -r`/build

default:
	$(MAKE) -C $(KDIR) M=$$PWD
	cp hello5.ko hello5.ko.unstripped
	$(CROSS_COMPILE)strip -g hello.ko
clean:
	$(MAKE) -C $(KDIR) M=$$PWD clean
%.s %.i: %.c
	$(MAKE) -C $(KDIR) M=$$PWD $@
endif
