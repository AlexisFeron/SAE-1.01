exoStock : stock.o testStock.o
	gcc stock.o testStock.o -o exoStock

stock.o : stock.h stock.c
	gcc stock.c -c

testStock.o : testStock.c
	gcc testStock.c -c

clean :
	rm *.o exoStock

doc :
	doxygen

