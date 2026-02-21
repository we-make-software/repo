savedcmd_server.ko := ld -r -m elf_x86_64 -z noexecstack --no-warn-rwx-segments --build-id=sha1  -T /root/linux/scripts/module.lds -o server.ko server.o server.mod.o .module-common.o
