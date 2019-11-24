obj-m = myhook.o

myhook-y += khook.o

KDIR = /lib/modules/$(shell uname -r)/build
PWD  = $(shell pwd)

default:
	$(MAKE) -C $(KDIR) M=$(PWD) modules
	
clean:
	rm -rf .tmp_versions *.ko *.o *.mod.c .*.cmd *.symvers

