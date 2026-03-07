savedcmd_server.mod := printf '%s\n'   main.o overflow/core.o network/core.o core/core.o | awk '!x[$$0]++ { print("./"$$0) }' > server.mod
