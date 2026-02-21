savedcmd_server.mod := printf '%s\n'   server.o | awk '!x[$$0]++ { print("./"$$0) }' > server.mod
