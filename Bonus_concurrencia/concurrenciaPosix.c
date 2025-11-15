/*********************************************************************************************
 * Pontificia Universidad Javeriana
 *Autores:Santiago Hernandez Morales, Jose Jesus Cepeda Vargas, Karol Dayan Torres Vides,Andres Eudardo Meneses Rincon.
 * Materia Sistemas Operativos
 * Docente: J. Corredor, PhD
 * Fecha: 14/11/2025
 * Tema: Posix para la creación de hilos concurrentes:
 *
 * Descripción:Este programa encuentra el maximo valor de un vector 
 * Cada hilo busca el maximo segmento de un vector para al final obtener un maximo global
 *
 * 
 *********************************************************************************************/
//se declaran las bibliotecas que se van a utilizar en el programa.
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "moduloconcurrenciaPosix.h"

void *buscarMax(void *parametro){ //se declara la funcion buscarMAX la cual tiene como proposito encontrar el maximo valor de un vector
    param_H *argumentos = (param_H *)parametro; //se declara la variable argumentos la cual es un puntero a la estructura param_h

    // 
    argumentos->maxparcial = argumentos->vector[argumentos->inicio]; //se inicialliza el maximo parcial con el primer elemento del segmento del vector

    // 
    for(int i = argumentos->inicio; i < argumentos->fin; i++){//se genera un ciclo for con el proposito de buscar el maximo valor en el segmento del vector
        if(argumentos->vector[i] > argumentos->maxparcial)//si el elemento actual es mayor al maximo parcial entonces se ejecuta lo siguiente
            argumentos->maxparcial = argumentos->vector[i];//se actualiza el maximo parcial con el valor del elemento actual
    }

    pthread_exit(NULL);//se sale del hilo
}

int maximoValor(int *vec, int n, int nhilos) { //se declara la funcion maximo valor la cual tiene como proposito encontrar el maximo valor de un vector

    pthread_t hilos[nhilos];//se declara la variable hilos la cual es un arreglo de hilos
    param_H datos[nhilos];//se delcara la variable de datos la cual es un arreglo de estructuras param_h

    int tam_bloque = n / nhilos;//se calcula el tamaño de cada bloque de datos que se va a procesar por cada hilo
    int resto = n % nhilos;//se calcula el resto de la division de n entre nhilos
    int inicio = 0;//se inicializa la variable inicio en 0

    // Crear los hilos
    for (int i = 0; i < nhilos; i++) {//se genera un ciclo for con el proposito de crear los hilos

        datos[i].inicio = inicio;//se asigna el inicio del bloque de datos que se va a procesar por el hilo
        datos[i].fin = inicio + tam_bloque + (i < resto ? 1 : 0);//se asigna el fin del bloque de datos que se va a procesar por el hilo
        datos[i].vector = vec;//se asigna el vector que se va a procesar por el hilo
        datos[i].maxparcial = vec[inicio];//se inicializa el maximo parcial con el primer elemento del bloque de datos

        inicio = datos[i].fin;//se actualiza el inicio del bloque de datos que se va a procesar por el hilo

        if (pthread_create(&hilos[i], NULL, buscarMax, &datos[i]) != 0) {//si el hilo no se crea correctamente entonces se genera un error
            perror("Error creando hilo");//se indica el error
            exit(-1);//se sale del programa
        }
    }

    // Esperar hilos
    for (int i = 0; i < nhilos; i++)//se crea un ciclo for con el proposito de esperar a que los hilos terminen
        pthread_join(hilos[i], NULL);//se espera a que el hilo termine


    int maxGlobal = datos[0].maxparcial;//se calcula el maximo global con el proposito de encontrar el maximo valor de los maximos parciales

    for (int i = 1; i < nhilos; i++)//se genera un ciclo for con el proposito de encontrar el maximo valor de los maximos parciales
        if (datos[i].maxparcial > maxGlobal)//si el maximo parcial es mayor al maximo global entonces se ejecuta lo siguiente
            maxGlobal = datos[i].maxparcial;//se actualiza el maximo global con el valor del maximo parcial

    return maxGlobal;//se retorna el maximo global
}

