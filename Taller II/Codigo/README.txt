Comandos necesarios para compilar los archivos:

test.c
	gcc test.c -o test -fopenmp 
pi.c
	gcc pi.c -o pi -fopenmp 
pi_loop.c
	gcc pi_loop.c -o pi_loop -fopenmp 
saxpy.c 
	gcc saxpy.c -o saxpy -fopenmp 
euler.c 
	gcc euler.c -o euler -fopenmp 
	
Luego se ejecuta el ejecutable :
	./<nombre .o
