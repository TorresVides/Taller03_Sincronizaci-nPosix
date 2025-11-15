/***************************************************************************************************************
*Autores: Santiago Hernandez Morales, Jose Jesus Cepeda Vargas, Karol Dayan Torres Vides,Andres Eudardo Meneses Rincon.
*Fecha: 14/11/2025
*Materia: Sistemas Operativos.
*Descripcion: el programa es una implementacion de un problema de cola de impresion mediante el uso de hilos POSIX, en este caso los 10 hilos productores generan
 cadenas de texto y el de cola de impresion las imprime, en este programa se utiliza mutex y un buffer para la sincronizacion de los hilos
**********************************************************************************************************************************/

//se declaran las bibliotecas que se van a utilizar en el programa.
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>      // O_CREAT, O_RDWR
#include <sys/mman.h>   // shm_open, mmap
#include <semaphore.h>  // sem_open, sem_wait, sem_post
#include <pthread.h>
#include<string.h>

//se declara una constante la cual va a representar la maxima cantidad de buffers que se van a utilizar en el programa
#define MAX_BUFFERS 10
#include "moduloposixSincro.h"

char buf [MAX_BUFFERS] [100]; //Es un buffer de 10 posiciones y en donde cada posicion tiene 100 caracteres
int buffer_index;//este indice indica donde se va a escribir el mensaje
int buffer_print_index;//este indice indica donde se va a imprimir el mensaje

pthread_mutex_t buf_mutex = PTHREAD_MUTEX_INITIALIZER; //Se utiliza buf_mutex con el proposito de proteger diferentes variables que se han compartido
pthread_cond_t buf_cond = PTHREAD_COND_INITIALIZER;//esta señal se utiliza cuando hay buffers disponibles
pthread_cond_t spool_cond = PTHREAD_COND_INITIALIZER;//esta señal se utiliza cuando hay lineas que se pueden imprimir
int buffers_available = MAX_BUFFERS;//representa un contador de buffers que se encuentran disponibles
int lines_to_print = 0;//es un contador de lineas listas para que la cola de impresion (spooler) las imprima

char buf [MAX_BUFFERS] [100]; //Es un buffer de 10 posiciones y en donde cada posicion tiene 100 caracteres
int buffer_index; //este indice indica donde se va a escribir el mensaje
int buffer_print_index;//este indice indica donde se va a imprimir el mensaje.



void *producer (void *arg); //esta funcion representa el hilo productor.
void *spooler (void *arg);//esta funcion representa el hilo spooler

int main (int argc, char **argv){ //se declara la funcion main la cual va a ejecutar el programa.
    pthread_t tid_producer [10], tid_spooler;//representa el arreglo de 10 hilos incluyendo el de spooler.
    int i, r;//se declaran las variables que se van a utilizar en los ciclos y los condicionales.

    buffer_index = buffer_print_index = 0; //se inicializan los indices del buffer en 0.
    if ((r = pthread_create (&tid_spooler, NULL, spooler, NULL)) != 0) { //si el hilo spooler no se genera correctamente entonces se genera un error
        fprintf (stderr, "Error = %d (%s)\n", r, strerror (r)); exit (1); //se indica el error y se sale del programa
    }
    int thread_no [10]; //Este arreglo indica los IDS de cada hilo productor
    for (i = 0; i < 10; i++) { //se genera un ciclo for el cual tiene el proposito de generar los 10 hilos productores
        thread_no [i] = i;//se asigna el ID a cada uno de los hilos productores
        if ((r = pthread_create (&tid_producer [i], NULL, producer, (void *) &thread_no [i])) != 0) { //si el hilo no se genera correctamente entonces se genera un error
            fprintf (stderr, "Error = %d (%s)\n", r, strerror (r)); exit (1); //se indica el error y se sale del programa
        }
    }
    for (i = 0; i < 10; i++) //se genera un ciclo for con el proposito de esperar a que los hilos productores terminen
        if ((r = pthread_join (tid_producer [i], NULL)) == -1) { //si el hilo no se ejecuto correctamente entonces se genera un error.
            fprintf (stderr, "Error = %d (%s)\n", r, strerror (r)); exit (1); //se indica el error y se sale del programa
        }

    while (lines_to_print) sleep (1); //se genera un ciclo while con el proposito de esperar a que el spooler termine de imprimir lo que falta.
    if ((r = pthread_cancel (tid_spooler)) != 0) { //si el hilo spooler no se ejecuta correctamente se genera un error.
        fprintf (stderr, "Error = %d (%s)\n", r, strerror (r)); exit (1);//se indica el error y se sale del programa
    }

    exit (0); //se sale del programa.
}

void *producer (void *arg){//se declara la funcion produce
    int i, r;//se declaran las variables que se van a utilizar en los ciclos y los condicionales
    int my_id = *((int *) arg);//se declara la variable my_id la cual representa el ID del hilo productor
    int count = 0;//se declara la variable count la cual representa el contador de linea que se van a imprimir

    for (i = 0; i < 10; i++) {//se genera un ciclo for con el proposito de producir 10 lineas de texto

        if ((r = pthread_mutex_lock (&buf_mutex)) != 0) {//si el mutex no se ejecuta correctamente se genera un error
            fprintf (stderr, "Error = %d (%s)\n", r, strerror (r)); exit (1);//se indica el error y se sale del programa
        }
            while (!buffers_available) //se gener un ciclo while con el proposito de esperar a que haya buffers disponibles
                pthread_cond_wait (&buf_cond, &buf_mutex);//se espera a que haya buffers disponibles

            int j = buffer_index;//se declara la variable j la cual representa el indice del buffer donde se va a escribir el mensaje
            buffer_index++;//se incrementa el indice del buffer
            if (buffer_index == MAX_BUFFERS)//si el indice del buffer es igual a la maxima cantidad de buffers entonces se ejecuta lo siguiente
                buffer_index = 0;//se inicializa el indice del buffer en 0
            buffers_available--;//se decrementa la cantidad de buffers disponibles

            sprintf (buf [j], "Thread %d: %d\n", my_id, ++count);//se escribe el mensaje en el buffer
            lines_to_print++; //se incrementa la cantidad de lineas que se van a imprimir

            pthread_cond_signal (&spool_cond);//se indica que hay lineas que se pueden imprimir

        if ((r = pthread_mutex_unlock (&buf_mutex)) != 0) {//si el mutex no se ejecuta correctamente se genera un error
            fprintf (stderr, "Error = %d (%s)\n", r, strerror (r)); exit (1);//se indica el error y se sale del progra,a
        }

        sleep (1);//se espera un segundo con el proposito de simular el tiempo de produccion
    }
    pthread_exit(NULL);//se sale del hilo
}


void *spooler (void *arg){//se declara la funcion spooler la cual tiene como proposito imprimir las lineas de texto.
    int r;//se declara la variable r la cual representa el resultado de la funcion pthread_mutex_lock

    while (1) {  //mientras haya lineas que se puedan imprimir se ejecuta el ciclo while
        if ((r = pthread_mutex_lock (&buf_mutex)) != 0) {//si el mutex no se ejecuta correctamente se genera un error
            fprintf (stderr, "Error = %d (%s)\n", r, strerror (r)); exit (1);//se indica el error y se sale del programa
        }
            while (!lines_to_print) //se genera un ciclo while con el proposito de esperar a que haya lineas que se puedan imprimir
                pthread_cond_wait (&spool_cond, &buf_mutex);//se espera a que haya lineas que se puedan imprimir

            printf ("%s", buf [buffer_print_index]);//se imprime la linea de texto
            lines_to_print--;//se disminuye la cantidad de lineas que se van a imprimir

            buffer_print_index++;//se incrementa el indice del buffer
            if (buffer_print_index == MAX_BUFFERS)//si el indice del buffer es igual a la maxima cantidad de buffers entonces se ejecuta lo siguiente
               buffer_print_index = 0;//se inicializa el indice del buffer en 0

            buffers_available++;//se incrementa la cantidad de buffers disponibles
            pthread_cond_signal (&buf_cond);//se indica que hay buffers disponibles

        if ((r = pthread_mutex_unlock (&buf_mutex)) != 0) {//si el mutex no se ejecuta correctamente se genera un error
            fprintf (stderr, "Error = %d (%s)\n", r, strerror (r)); exit (1);//se indica el error y se sale del programa.
        }

    }
}