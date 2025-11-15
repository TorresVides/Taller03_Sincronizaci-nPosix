/***************************************************************************************************************
*Autores: Santiago Hernandez Morales, Jose Jesus Cepeda Vargas, Karol Dayan Torres Vides,Andres Eudardo Meneses Rincon.
*Fecha: 14/11/2025
*Materia: Sistemas Operativos.
*Descripcion: El programa utiliza semaforos y memoria compartida POSIX con el proposito de leer datos producidos por un proceso.
Pero tambien se implementa el uso de un buffer que tiene un tamaño fijo con el proposito de implementar el patron de productor consumidor.
**********************************************************************************************************************************/

//se declaran las bibliotecas que se van a utilizar en el programa
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>      // O_CREAT, O_RDWR
#include <sys/mman.h>   // shm_open, mmap
#include <semaphore.h>  // sem_open, sem_wait, sem_post

//se declara una constante la cual representa el tamaño del buffer
#define BUFFER 10

typedef struct{ //se genera una estructura de datos compartida entre productor y consumidor
    int salida; //la variable salida representa el indice del siguiente elemento que se va a consumir
    int bus[BUFFER]; //en este buffer es donde se van a almacenar los elementos.
}compartir_datos; //nombre de la estructura

//se declara la funcion principal la cual va a jecutar el programa
int main() {
    sem_t *vacio = sem_open("/vacio", 0);//se abre el semaforo vacio el cual fue creado por el productor
    sem_t *lleno = sem_open("/lleno", 0);//se abre el semaforo lleno que fue creado por el productor.
    if (vacio == SEM_FAILED || lleno == SEM_FAILED) {//si los semaforos no se abren de manera correcta entonces se genera un error
        perror("sem_open");//indica el error al cumplirse la condicion
        exit(EXIT_FAILURE);//se sale del programa debido a que los semaforos no se abrieron de forma correcta.
    }

    int fd_compartido = shm_open("/memoria_compartida", O_RDWR, 0644); //se abre la memoria compartida la cual fue creada por el productor
    if (fd_compartido < 0) {//si la memoria compartida no se abrio correctamente entonces se genera un error
        perror("shm_open");//indica el error al cumplirse la condicion
        exit(EXIT_FAILURE);//se sale del programa debido a que la memoria compartida no se abrio correctamente
    }

    compartir_datos *compartir = mmap(NULL, sizeof(compartir_datos), PROT_READ | PROT_WRITE, MAP_SHARED, fd_compartido, 0);//se mapea la memoria compartida-
    compartir->salida = 0; //se inicializa la variable de salida en 0.

    for (int i = 1; i <= 10; i++) { //se genera un ciclo for el cual inicia en 1 y termina en 10 el cual tiene el proposito de consumir los datos producidos.
        sem_wait(lleno);  //se espera a que el semaforo este disponible 
        int item = compartir->bus[compartir->salida]; //se lee el elemento del buffer que se encuentra en la posicion de salida.
        printf("Consumidor: Consume %d\n", item);//se imprime el elemento que se consumio

        compartir->salida = (compartir->salida+1) % BUFFER; //se avanza el indice de salida con el proposito de consumir el siguiente elemento

        sem_post(vacio);//se indica que aun hay espacio en el buffer
        sleep(2);  //se espera 2 segundos con el proposito de simular el procesamiento
    }

    munmap(compartir, sizeof(compartir_datos));//se deja de mapear la memoria compartida
    close(fd_compartido);//se cierra la memoria compartida.
    sem_close(lleno); //se cierra el semaforo lleno
    sem_unlink("/lleno");//se elimina el semaforo lleno
    shm_unlink("/memoria_compartida");//se elimina la memoria compartida
    return 0; //se retorna 0 con el proposito de indicar que el programa se ejecuto correctamente.
}