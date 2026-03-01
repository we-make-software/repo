obj-m := server.o
server-objs := main.o overflow/core.o EUI48/core.o core/system.o network/core.o IEEE802_3/core.o
ccflags-y := -Iinclude -O2 -Wall 

build:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

all:
	rmmod server 2>/dev/null || true
	make build
	insmod server.ko



clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

stop:
	rmmod server 2>/dev/null || true
	$(MAKE) clean


start:
	dmesg -C
	clear
	rm -rf -- "/root/repo/server/debug";
	mkdir -p -- "/root/repo/server/debug"
	make all
	dmesg -w

pp:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) server.i

github:
	git add -A
	git commit -m "Update" || true
	git push