build:
	gcc -shared -fPIC -Idiscord-rpc/linux-dynamic/include/ -Ldiscord-rpc/linux-dynamic/lib/ -ldiscord-rpc rpc.c -o rpc.so