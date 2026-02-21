
obj-m := server.o
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

log:
	dmesg -C
	clear
	dmesg -w

start:
	rm -rf -- "/root/repo/server/debug";
	mkdir -p -- "/root/repo/server/debug"
	make all
	make log

pp:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) server.i

github:
	git add -A
	git commit -m "Update" || true
	git push