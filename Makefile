CC=gcc
CFLAGS=-ldiscord-rpc

build:
	gcc rpc.c -g discord-rpc/linux-dynamic/lib/libdiscord-rpc.so -o micro_rpc

library:
	gcc -shared -fPIC rpc.c -o rpc.so
