savedcmd_server.mod := printf '%s\n'   main.o core/system.o network/core.o | awk '!x[$$0]++ { print("./"$$0) }' > server.mod
