ifneq ($(KERNELRELEASE),)

SERVER_DIRS := $(shell cat project_manager)
SERVER_FILES := $(addsuffix /core.o,$(SERVER_DIRS))

obj-m += server.o
server-y := main.o core/core.o
ccflags-y := -Iinclude -O2 -Wall

else

KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

build:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

all:
	rmmod server 2>/dev/null || true
	$(MAKE) build
	insmod ./server.ko

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean

stop:
	rmmod server 2>/dev/null || true
	$(MAKE) clean

start:
	dmesg -C || true
	clear || true
	rm -rf -- "$(PWD)/debug"
	mkdir -p -- "$(PWD)/debug"
	$(MAKE) all
	dmesg -w

github:
	git add -A
	git commit -m "Update" || true
	git push

endif