CC=gcc
CFLAGS=-ldiscord-rpc -Ldiscord-rpc/linux-dynamic/lib -Hdiscord-rpc/linux-dynamic/include

build:
	${CC} ${CFLAGS} rpc.c -o micro_rpc -Wl,-rpath,./discord-rpc/linux-dynamic/lib
