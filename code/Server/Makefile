ifneq ($(KERNELRELEASE),)

obj-m += server.o
server-y := main.o Debug/core.o Common/core.o Common/Sync/core.o Hardware/core.o Hardware/Memory/core.o Hardware/Network/core.o Hardware/Storage/core.o Hardware/Storage/Block/core.o Hardware/Storage/Config/core.o Hardware/Storage/Delete/core.o Hardware/Storage/Upgrade/core.o Network/core.o Network/Device/core.o OSI/core.o OSI/EUI48/core.o OSI/RFC7042/core.o OSI/RFC2464/core.o OSI/IEEE8023/core.o OSI/RFC791/core.o OSI/RFC8200/core.o OSI/RFC826/core.o OSI/RFC4443/core.o
 
ccflags-y := -Iinclude -O2 -Wall

else

KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)
msg ?=


build:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

all:
	rmmod server 2>/dev/null || true
	$(MAKE) build
	insmod ./server.ko

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean

log:
	dmesg -C || true
	clear || true
	dmesg -w || true

stop:
	clear || true
	rmmod server 2>/dev/null || true
	$(MAKE) clean
	
start:
	clear || true
	rmmod server 2>/dev/null || true
	rm -rf -- "$(PWD)/debug"
	mkdir -p -- "$(PWD)/debug"
	$(MAKE) build
	insmod ./server.ko


github:
	$(MAKE) clean
	@if [ -z "$(msg)" ]; then \
		echo "Error: commit message is required. Use: make github msg=\"your message\""; \
		exit 1; \
	fi
	git add -A
	git commit -m "$(msg)"
	git push



UpdateOS:
	cd ~/gcc && git pull
	cd ~/gcc/build && $(MAKE) -j"$$(nproc)" && $(MAKE) install
	hash -r
	gcc --version
	cd ~/linux && \
	git fetch --tags origin && \
	git checkout "$$(git tag -l 'v*-rc*' --sort=-version:refname | head -n 1)" && \
	cp -v /boot/config-$$(uname -r) .config && \
	$(MAKE) olddefconfig && \
	$(MAKE) -j"$$(nproc)" && \
	$(MAKE) modules_install && \
	$(MAKE) install && \
	reboot

endif
