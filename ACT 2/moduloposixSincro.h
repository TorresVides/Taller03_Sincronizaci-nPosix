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

#ifndef __MODULOPOSIXSINCRO_H__//se genera un ifndef con el proposito de evitar que el archivo se incluya mas de una vez
#define __MODULOPOSIXSINCRO_H__
//se genera un define

//se declaran las bibliotecas que se van a utilizar en el programa
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>    
#include <sys/mman.h>  
#include <semaphore.h>  
#include <pthread.h>

//se define una constante la cual representa el tamaño del buffer
#define MAX_BUFFERS 10

extern char buf [MAX_BUFFERS] [100];//se declara el buffer el cual es un arreglo de 10 posiciones y en donde cada posicion tiene 1000 caracteres
extern int buffer_index; //este indice indica donde se va a escribir el mensaje
extern int buffer_print_index; //este indice indica donde se va a imprimir el mensaje

extern pthread_mutex_t buf_mutex;//este mutex se utiliza para proteger la variables compartidas
extern pthread_cond_t buf_cond; //esta señal se utiliza cuando hay buffers disponibles
extern pthread_cond_t spool_cond;//esta señal se utiliza cuando hay lineas que se pueden imprimir
extern int buffers_available;//representa un contador de buffers que se encuentran disponibles
extern int lines_to_print;//es un contador de lineas listas para que la cola de impresion (spooler) las imprima

void *producer (void *arg);//esta funcion representa el hilo productor
void *spooler (void *arg);//esta funcion representa el hilo spooler

#endif //se cierra el ifndef