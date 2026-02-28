savedcmd_server.o := ld -m elf_x86_64 -z noexecstack --no-warn-rwx-segments   -r -o server.o @server.mod  ; /root/linux/tools/objtool/objtool --hacks=jump_label --hacks=noinstr --hacks=skylake --ibt --orc --retpoline --rethunk --sls --static-call --uaccess --prefix=16  --link  --module server.o

server.o: $(wildcard /root/linux/tools/objtool/objtool)
