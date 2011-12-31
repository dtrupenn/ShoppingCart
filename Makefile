shopper : cart.c
	clang -o shopper cart.c

run : shopper
	./shopper output.txt