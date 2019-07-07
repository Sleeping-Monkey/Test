CURRENT = $(shell uname -r)
KDIR = /lib/modules/$(CURRENT)/build 
PWD = $(shell pwd) 
TARGET = md1
obj-m   := $(TARGET).o

default:
	$(MAKE) -C $(KDIR) M=$(PWD) modules 

clean:
	@rm -f *.o .*.cmd .*.flags *.mod.c *.order
	@rm -f .*.*.cmd *~ *.*~ TODO.* 
	@rm -fR .tmp*
	@rm -rf .tmp_versions 

fclean: clean
	@rm *.ko *.symvers

re: fclean default
