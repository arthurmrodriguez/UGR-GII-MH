# fichero Makefile
#
# nombre del fichero ejecutable
#
TARGETS= ../BIN/Practica2

# banderas para el compilador (C y C++). Se indican los directorios donde se puenden
# encontrar ficheros para incluir mediante la opcion -I
# la opcion -g sirve para permitir la depuración
#
CFLAGS=    -g -I/usr/include -I.
CXXFLAGS=  -g -w -I/usr/include -I. -std=c++11 -O2

# definicion del compilador
#
CC = g++

# orden que sera ejecutada por defecto
#
default : $(TARGETS)

# regla de ejecucion
#
$(TARGETS) : Practica2.o ConjuntoDatos.o Utilidades.o
	$(CC) -o $@ $(CXXFLAGS) Practica2.o  ConjuntoDatos.o Utilidades.o

#
#
clean:
	rm -f *.o
	rm -f $(TARGETS)
