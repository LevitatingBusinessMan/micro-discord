CC=gcc
CFLAGS=-Ldiscord-rpc/linux-dynamic/lib -Idiscord-rpc/linux-dynamic/include
LDLIBS=-ldiscord-rpc 

build:
	${CC} ${CFLAGS} rpc.c ${LDLIBS} -o micro_rpc -Wl,-rpath,./discord-rpc/linux-dynamic/lib
