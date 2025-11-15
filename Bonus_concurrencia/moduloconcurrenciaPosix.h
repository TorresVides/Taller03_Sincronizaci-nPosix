/*********************************************************************************************
 * Pontificia Universidad Javeriana
 *Autores:Santiago Hernandez Morales, Jose Jesus Cepeda Vargas, Karol Dayan Torres Vides,Andres Eudardo Meneses Rincon.
 * Materia Sistemas Operativos
 * Docente: J. Corredor, PhD
 * Fecha: 14/11/2025
 * Tema: Posix para la creación de hilos concurrentes:
 *
 * Descripción:en este archivo se declaran las funciones y estrcutras que se van a utilizar en el programa concurrenciaPosix.c
 *
 * 
 *********************************************************************************************/

#ifndef __MODULOCONCURRENCIAPOSIX_H__//se genera un ifndef con el proposito de evitar que el archivo se incluya mas de una vez
#define __MODULOCONCURRENCIAPOSIX_H__
//se genera un define

//se declaran las bibliotecas que se van a utilizar en el programa
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

struct argHilos { //se genera una estructura la cual va a contener los argumentos que se van a pasar a los hilos
    int inicio;//representa el inicio del segmento del vector que se va a procesar por el hilo
    int fin;//representa el fin del segmento del vector que se va a procesar por el hilo
    int *vector;//representa el vector que se va a procesar por el hilo
    int maxparcial;//representa el maximo valor parcial que se encuentra en el segmento del vector que se va a procesar por el hilo
};

typedef struct argHilos param_H; //se genera un typedef con el proposito de renombrar la estructura argHilos como param_h

int maximoValor(int *vec, int n, int nhilos);//se declara la funcion maximo valor con el proposito de encontrar el maximo valor de un vector
void *buscarMax(void *parametro);//se declara la funcion buscarMax con el proposito de encontrar el maximo valor de un segmento del vector

#endif //se cierra el ifndef