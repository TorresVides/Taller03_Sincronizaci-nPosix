############################################################
#		Pontificia Universidad Javeriana
#		Sistemas Operativos
#		Autores:Santiago Hernandez Morales, Jose Jesus Cepeda Vargas, Karol Dayan Torres Vides,Andres Eudardo Meneses Rincon.
#		Docente: J. Corredor
#		Descripción: Pemire la automatizacion de la compilacion de los programas
#
#############################################################

GCC = gcc
CFLAGS =
RM = -rf

PROGRAMAS = consumer concurrenciaPosix posixSincro producer

ALL: $(PROGRAMAS)

consumer:
	$(GCC) $@.c -o $@ $(FLAGS) 

concurrenciaPosix:
	$(GCC) modulo$@.c $@.c -o $@ $(FLAGS) 

posixSincro:
	$(GCC) modulo$@.c $@.c -o $@ $(FLAGS) 

producer:
	$(GCC) $@.c -o $@ $(FLAGS) 

# Comando dinamico
%: %.c
	$(GCC) $< -o $@ $(FLAGS)

clean:
	rm $(RM) $(PROGRAMAS)